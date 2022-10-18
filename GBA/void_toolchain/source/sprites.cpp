#include "../include/sprites.h"

u16 SPRITE_TOTAL_OAM = 0;
OBJ_ATTR OBJ_BUFFER[ MAX_SPRITES_IN_OAM ];

void initOam(){ 
    oam_init(OBJ_BUFFER, MAX_SPRITES_IN_OAM); 
}

void updateOam(){ 
    oam_copy(oam_mem, OBJ_BUFFER, SPRITE_TOTAL_OAM); 
}

u8 checkSpriteIsHided( u16 id ){
    if( OBJ_BUFFER[ id ].attr0 & ATTR0_HIDE ){
        return TRUE;
    }
    return FALSE;
}

Sprite::Sprite( s16 slot ){
    if( slot == -1 ){
        SPRITE_TOTAL_OAM++;
        slot_in_oam = SPRITE_TOTAL_OAM - 1;
        spr = &OBJ_BUFFER[ slot_in_oam ];
    }else {
        vu16 tmp, tmp2;

        if( slot > 0 ){
            tmp = slot - ( SPRITE_TOTAL_OAM - 1 );
            tmp2 = SPRITE_TOTAL_OAM + tmp;
            SPRITE_TOTAL_OAM = tmp2;
        }else {
            SPRITE_TOTAL_OAM = 1;
        }

        spr = &OBJ_BUFFER[ slot ];
        slot_in_oam = slot;
    }
}

void Sprite::update(){
    tick++;

    if( flip ){
        spr->attr1 |= ATTR1_HFLIP;
    }else if( !flip ){
        spr->attr1 &= 0xefff;
    }

    obj_set_attr( spr, spr->attr0, spr->attr1, ATTR2_BUILD(tid, pal, prio) );
    obj_set_pos( spr, pos.x, pos.y );
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

    if( spr->attr1 & ATTR1_SIZE_32x32 ){
        size = 16;
    }else if( spr->attr1 & ATTR1_SIZE_16x16 ){
        size = 4;
    }else if( spr->attr1 & ATTR1_SIZE_8x8 ){
        size = 1;
    }

    start = start * size;

    if( tid < start ){
        tid = start * size;
    }

    u16 id = tid - start;
    if( tick % rate == 0 ){
        id = ( id + size ) % ( frames * size );
    }

    tid = start + id;
}

void Sprite::setTileId( u16 id ){
    vu16 size;

    if( spr->attr1 & ATTR1_SIZE_16 ){
        size = 2;
    }else if( spr->attr1 & ATTR1_SIZE_8 ){
        size = 1;
    }

    tid = id * size;
}
