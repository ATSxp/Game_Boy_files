#include "../include/void.h"

u16 SPRITE_TOTAL_OAM = 0;
OBJ_ATTR OBJ_BUFFER[ MAX_SPRITES_IN_OAM ];

u16 SPRITE_IN_VRAM_OBJ = 0;
u16 SPRITE_IN_VRAM_TILE = 0;
u16 PAL_IN_BANK_OBJ = 0;
u16 PAL_IN_BANK_BG = 0;

int shake_t = 0;
u32 shake = 0, shake_bg = 0, shake_mag;
BOOL shaking = FALSE;
void shakeScreen( u32 amt, u32 bg, u32 mag){
    if( amt > shake ){ shake = amt; }
    shake_bg = bg;
    shake_mag = mag;
}

void updateShakeScreen(){
    shake_t++;

    if( shake > 0 ){
        shaking = TRUE;
        shake--;
        REG_BG_OFS[ shake_bg ].x = -( ( shake_t >> 1 ) % 2 * shake_mag );
    }else {
        shaking = FALSE;
        REG_BG_OFS[ shake_bg ].x = 0;
    }

}

void tte_write_str( std::string text ){ tte_write( text.c_str() ); }
void nocash_puts_str(std::string text){ nocash_puts( text.c_str() ); }

static BOOL scene_changed = FALSE;
static void nothing(void){};

static Scene next_scene = {
    nothing,
    nothing,
    nothing
};

static Scene cur_scene = {
    nothing,
    nothing,
    nothing
};

void setScene(Scene next){
    scene_changed = TRUE;
    next_scene = next;
}

void updateScene(){
    if( scene_changed ){
        scene_changed = FALSE;
        cur_scene.end();
        cur_scene = next_scene;
        cur_scene.init();
    }
    cur_scene.update();
}

void initVoid(){
    initOam();
}

void updateVoid(){
    updateOam();   
}

void resetVoid(){
    SPRITE_TOTAL_OAM = 0;
    SPRITE_IN_VRAM_OBJ = 0;
    SPRITE_IN_VRAM_TILE = 0;
    PAL_IN_BANK_OBJ = 0;
    PAL_IN_BANK_BG = 0;
}

u8 aabb( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 ){
    if( ( x1 < x2 + w2 && y1 < y2 + h2 && x2 < x1 + w1 && y2 < y1 + h1 ) == true ){
        return TRUE;
    }

    return FALSE;
}

u16 mget(int x, int y, cu16 *bg, u16 map_width){
    return bg[y * map_width + x];
}

void mset(int x, int y, u8 t, u16 *bg, u16 map_width){
}

u8 spriteIsHided( u16 slot ){
    if( ( OBJ_BUFFER[ slot ].attr0 & ATTR0_MODE_MASK ) == ATTR0_HIDE ){
        return TRUE;
    }
    return FALSE;
}

void initOam(){ 
    oam_init(OBJ_BUFFER, MAX_SPRITES_IN_OAM); 
}

void updateOam(){ 
    oam_copy(oam_mem, OBJ_BUFFER, SPRITE_TOTAL_OAM); 
}

// Class Sprite /////////////////////////////////////////
Sprite::Sprite( s16 slot ){
    if( slot != -1 ){
        newSprite(slot);
    }
}

void Sprite::newSprite( u16 slot ){
    spr = &OBJ_BUFFER[ slot ];
    slot_in_oam = slot;
}

void Sprite::update(){
    tick++;

    if( flip_h ){
        spr->attr1 |= ATTR1_HFLIP;
    }else if( !flip_h ){
        spr->attr1 &= 0xefff;
    }

    /* if( flip_v ){ */
    /*     spr->attr1 |= ATTR1_VFLIP; */
    /* }else if( !flip_v ){ */
    /*     spr->attr1 &= 0xefff; */
    /* } */

    size.w = getWidth();
    size.h = getHeight();

    obj_set_attr( spr, spr->attr0, spr->attr1, ATTR2_BUILD(tid, pal, prio) );
    obj_set_pos( spr, pos.x, pos.y );
}

Size Sprite::getSize(){
    vu32 _w = obj_get_width(spr), _h = obj_get_height(spr);
    return { _w, _h };
}

int Sprite::getWidth(){
    return obj_get_width(spr);
}

int Sprite::getHeight(){
    return obj_get_height(spr);
}

void Sprite::setAttr(u16 attr0, u16 attr1){
    spr->attr0 = attr0;
    spr->attr1 = attr1;
}

void Sprite::setPos( int nx, int ny ){
    pos.x = nx;
    pos.y = ny;
}

Axis Sprite::getPos(){
    return { pos.x, pos.y };
}

void Sprite::hide(){
    obj_hide(spr); 
}

void Sprite::unhide(){
    obj_unhide(spr, 0); 
}

void Sprite::anim(u16 start, u16 frames, u16 rate){
    frames = frames == 0 ? 1 : frames;
    rate = rate ? rate : 1;
    u16 size = 1;

    if( getWidth() == 32 ){
        size = 8;
    }else if( getWidth() == 16 ){
        size = 4;
    }else if( getWidth() == 8 ){
        size = 1;
    }

    start = start * size;

    if( tid < start ){
        tid = start;
    }

    u16 id = tid - start;
    if( tick % rate == 0 ){
        id = ( id + size ) % ( frames * size );
    }

    tid = start + id;
}

void Sprite::setTileId( u16 id ){
    vu16 size;

    switch( getWidth() ){
        case 16:
            size = 4;
            break;
        case 8:
            size = 1;
            break;
    }

    this->tid = id * size;
}

// Class Map ////////////////////////////////////////////

Map::Map( int bgnr, u32 ctrl, const void *map, 
    u32 map_w, u32 map_h )
{
    flags = bgnr;
    cnt = ctrl;
    dst = se_mem[ BFN_GET( ctrl, BG_SBB) ];

    REG_BGCNT[ bgnr ] = ctrl;
    REG_BG_OFS[ bgnr ].x = 0;
    REG_BG_OFS[ bgnr ].y = 0;

    src = (SCR_ENTRY*)map;
    w = map_w;
    h = map_h;

    SCR_ENTRY *_dst = dst, *_src = src;
    for( int iy = 0; iy < 32; iy++ ){
        for( int ix = 0; ix < 32; ix++ ){
            _dst[ iy * 32 + ix ] = _src[ iy * w + ix ];
        }
    }
}

void Map::colcpy( int tx, int ty ){
    int iy, y0 = ty & 31;

    int srcP = w;
    SCR_ENTRY *srcL = &src[ ty * srcP + tx ];
    SCR_ENTRY *dstL = &dst[ y0 * 32 + ( tx & 31 ) ];
    
    for( iy = y0; iy < 32; iy++ ){
        *dstL = *srcL;
        dstL += 32;
        srcL += srcP;
    }

    dstL -= 1024;

    for( iy = 0; iy < y0; iy++ ){
        *dstL = *srcL;
        dstL += 32;
        srcL += srcP;
    }
}

void Map::rowcpy( int tx, int ty ){
    int ix, x0 = tx & 31;

    int srcP = w;
    SCR_ENTRY *srcL = &src[ ty * srcP + tx ];
    SCR_ENTRY *dstL = &dst[ ( ty & 31 ) * 32 + x0 ];
    
    for( ix = x0; ix < 32; ix++ ){
        *dstL++ = *srcL++;
    }

    dstL -= 32;

    for( ix = 0; ix < x0; ix++ ){
        *dstL++ = *srcL++;
    }
}

void Map::update(){
    int vx = vp.x, vy = vp.y;
    int bx = x, by = y;

    int tvx = vx >> 3, tvy = vy >> 3;
    int tbx = bx >> 3, tby = by >> 3;

    if( tvy < tby ){
        rowcpy( tvx, tvy );
    }else if( tvy > tby ){
        rowcpy( tvx, tvy + 31 );
    }

    if( tvx < tbx ){
        colcpy( tvx, tvy );
    }else if( tvx > tbx ){
        colcpy( tvx + 31, tvy );
    }

    x = vx;
    y = vy;

    REG_BG_OFS[ flags ].x = x;
    REG_BG_OFS[ flags ].y = y;
}

void Map::setPos(int x, int y){
    vp.x = clamp( x, vp.xmin, vp.xmax - vp.xpage);
    vp.y = clamp( y, vp.ymin, vp.ymax - vp.ypage);
}

u16 Map::getTile(int tx, int ty){
    return mget( tx, ty, src, w);
}

void Map::setTile(int tx, int ty, u8 t){
    SCR_ENTRY *_dst = dst, *_src = src;

    _src[ y * w + x ] = t;
    _dst[ y * w + x ] = _src[ y * w + x ];
}

// Class Fade ////////////////////////////////////////////
/* Fade::Fade(const COLOR *src, COLOR clr, u32 num_clrs){ */
/*     this->src = src; */
/*     this->clr = clr; */
/*     this->num_clrs = num_clrs; */
/* } */

/* void Fade::update(){ */
/*     clr_fade(src, clr, pal_bg_mem, num_clrs, value); // Bgs */
/*     clr_fade(src, clr, pal_obj_mem, num_clrs, value); // Objs */
    
/*     value = clamp(value + vel, 0, 32); */
/* } */

/* void Fade::fadeIn( u16 spd ){ */
/*     value = 0; */
/*     vel = spd; */
/* } */

/* void Fade::fadeOut( u16 spd ){ */
/*     value = 32; */
/*     vel = -spd; */
/* } */

/* u16 fade_alpha_value; */
/* int fade_alpha_d; */

/* void updateFade(const COLOR *src, COLOR clr, u32 num_clrs){ */
/* } */

/* void fadeOut(u16 spd){ */
/*     fade_alpha_value = 32; */
/*     fade_alpha_d = -spd; */
/* } */

/* void fadeIn(u16 spd){ */
/*     fade_alpha_value = 0; */
/*     fade_alpha_d = spd; */
/* } */
