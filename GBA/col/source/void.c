// Void | Toolbox
#include "../include/void.h"

int TICK = 0;
int NUM_OBJS = 0; // Número de Objetos instanciados
uint LIMIT_OBJS = 128;
OBJ_ATTR obj_buffer[128]; // Buffer de Objetos, até 128
int MGET_WIDTH = 32;

void initVoid(){
    vid_vsync();
    key_poll();
    TICK++;
}

void initOam(){
    oam_init(obj_buffer, LIMIT_OBJS); // Inicia a OAM com um buffer pré-defenido
}

void loadObjectsToOam(){ oam_copy(oam_mem, obj_buffer, NUM_OBJS); }

uint getMode(){
    return REG_DISPCNT;
}

void setMode(uint mode){
    REG_DISPCNT = mode;
}

OBJ_ATTR *spr(uint id, int x, int y, bool f, uint shape, uint bpp, uint size){
    NUM_OBJS++; // Mais um Objeto adicionado ao game

    OBJ_ATTR *sprite = &obj_buffer[NUM_OBJS - 1];

    switch(bpp){
        case 8:
            sprite->attr0 = ATTR0_8BPP;
            break;
        case 4:
            sprite->attr0 = ATTR0_4BPP;
            break;
    }

    switch(shape){
        case SQUARE:
            sprite->attr0 |= ATTR0_SQUARE;
            break;
    }

    uint div_sz = 4;
    switch(size){
        // TODO: Fazer testes ( arrumar pelo menos dois sprite de 64x64 )
        /* case 64: */
        /*     sprite->attr1 = ATTR1_SIZE_64x64; */
        /*     div_sz = 16; */
        /*     break; */
        case 32:
            sprite->attr1 = ATTR1_SIZE_32x32;
            div_sz = 1;
            break;
        case 16:
            sprite->attr1 = ATTR1_SIZE_16x16;
            div_sz = 2;
            break;
        case 8:
            sprite->attr1 = ATTR1_SIZE_8x8;
            div_sz = 4;
            break;
    }

    // Flip
    flipSprite(sprite, f);

    vu32 spr_size = ( id * size ) / div_sz;
    sprite->attr2 = spr_size;
    
    obj_set_attr(sprite, sprite->attr0, sprite->attr1, sprite->attr2);
    obj_set_pos(sprite, x, y);

    return sprite;
}

void flipSprite(OBJ_ATTR *o, bool f){
    if( f ){
        o->attr1 |= ATTR1_HFLIP;
    }else if( !f ){
        o->attr1 &= 0xefff;
    }
}

void setObjectPosition(OBJ_ATTR *o, int x, int y){
    obj_set_pos(o, x, y); 
}

void setAnim(OBJ_ATTR *o, uint start, uint frames, uint delay){
    frames = frames == 0 ? 1 : frames;
    delay = delay ? delay : 1;

    uint spr_size = 1;

    if( o->attr1 & ATTR1_SIZE_32x32 ){
        spr_size = 16;
    }else if( o->attr1 & ATTR1_SIZE_16x16 ){
        spr_size = 4;
    }

    start = start * spr_size;

    if(o->attr2 < start){
        o->attr2 = start;
    }

    uint id = o->attr2 - start;
    if(TICK % delay == 0){
        id = ( id + spr_size ) % ( frames * spr_size );
    }

    o->attr2 = start + id;
}

void moveBg(uint bg, int x, int y){
    switch(bg){
        case 0:
            REG_BG0HOFS = -x;
            REG_BG0VOFS = -y;
            break;
        case 1:
            REG_BG1HOFS = -x;
            REG_BG1VOFS = -y;
            break;
        case 2:
            REG_BG2HOFS = -x;
            REG_BG2VOFS = -y;
            break;
        case 3:
            REG_BG3HOFS = -x;
            REG_BG3VOFS = -y;
            break;
    }
}

int PixelToTileX(int x){ return x * 8; }
int PixelToTileY(int y){ return y * 8; }

int TileToPixelX(int x){ return x / 8; }
int TileToPixelY(int y){ return y / 8; }

u8 mget(int x, int y, cu16 *bg){
    return bg[y * MGET_WIDTH + x];
}
