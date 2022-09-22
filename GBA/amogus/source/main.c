#include "tonc.h"
#include <string.h>

#include "amogus_idle.h"
#include "amogus_run.h"
#include "amogus_blue.h"
#include "tileset_map.h"
#include "tileset_map2.h"
#include "img_menu_bmp.h"
#include "map.h"

int state = 0;
int tick = 0; // Ticks
int mx = 0, my = 0;//32;

OBJ_ATTR obj_buffer[128]; // Objects
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer; // Objects affine

void loadSprite(int index, cu16 *tiles, cu16 tilesLen, cu16 *pal, int palLen){
    memcpy(&tile_mem[4][index], tiles, tilesLen); // Load sprite sheet
    memcpy(pal_obj_mem, pal, palLen); // Load palette
}

void loadTileset(int cbb, int index, cu16 *tileset, int tilesetLen, cu16 *pal, int palLen){
    memcpy(pal_bg_mem, pal, palLen);
    memcpy(&tile_mem[cbb][index], tileset, tilesetLen);
}

void loadMap(int sbb, int cbb, cu16 *map, int mapLen){
    memcpy16(&se_mem[sbb][cbb], tilemap, MAP_LENGTH);
}

void changeState(int next_state){
    state = next_state;
    RegisterRamReset(RESET_VRAM);
}

// Entities
typedef struct GameObject{
    OBJ_ATTR *sprite;
    int x; int y;
    int dx; int dy;
    int w; int h;
    int anim_len;
    int flip;
}GameObject;

void initAmogus(GameObject *obj, int x, int y, vu16 buffer, u16 attr0, u16 attr1, u16 attr2){
    obj->x = x;
    obj->y = y;
    obj->sprite = &obj_buffer[buffer];
    obj->w = 32;
    obj->h = 32;

    obj_set_attr(obj->sprite, attr0, attr1, attr2);
    obj_set_pos(obj->sprite, obj->x - mx, obj->y - my);
}

void updateAmogus(GameObject *obj, int dx, int dy){
    obj->dx = dx;
    obj->dy = dy;

    if(obj->dx != 0){
        obj->flip = obj->dx < 0 ? 1 : 0;
    }

    if(obj->flip){
        obj->sprite->attr1 |= ATTR1_HFLIP;
    }else {
        obj->sprite->attr1 &= 0xefff;
    }

    obj->x += obj->dx;
    obj->y += obj->dy;

    obj_set_pos(obj->sprite, obj->x - mx, obj->y - my);
}

void animAmogus(GameObject *obj, int start, int len){
    if(len == 0){
        len = 1;
    }

    if(obj->sprite->attr2 < start){
        obj->sprite->attr2 = start;
    }

    int id = obj->sprite->attr2 - start;
    if(tick % 8 == 0){
        id = ( id + 16 ) % len;
    }

    obj->sprite->attr2 = start + id;
}

GameObject am; // Amongus 1
GameObject am2; // Amongus 2

void initMenu(){
    if(state != 0){
        return;
    }

    memcpy(vid_mem, img_menu_bmpBitmap, img_menu_bmpBitmapLen);

    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;
}

void initGame(){
    if(state != 1){
        OAM_CLEAR();
        return;
    }

    loadTileset(0, 0, tileset_map2Tiles, tileset_map2TilesLen, tileset_map2Pal, tileset_map2PalLen);
    loadMap(30, 0, tilemap, MAP_LENGTH);

    loadSprite(128, amogus_idleTiles, amogus_idleTilesLen, amogus_idlePal, amogus_idlePalLen);
    loadSprite(0, amogus_runTiles, amogus_runTilesLen, amogus_runPal, amogus_runPalLen);
    loadSprite(64, amogus_blueTiles, amogus_blueTilesLen, amogus_bluePal, amogus_bluePalLen);

    oam_init(obj_buffer, 128); // Init OAM
                               
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_REG_32x32;
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;

    vu16 x = ( ( MAP_WIDTH * 8 ) - 32 ) / 2, y = ( ( MAP_HEIGHT * 8 ) - 32 ) / 2;

    initAmogus(&am, x, y, 0, ATTR0_SQUARE, ATTR1_SIZE_32x32, 0);
    initAmogus(&am2, 8, 8, 1, ATTR0_SQUARE, ATTR1_SIZE_32x32 | ATTR1_HFLIP, 5*16);

    /* tte_init_se_default(0, BG_CBB(0) | BG_SBB(30)); */
    /* tte_write("#{P:90, 10}"); */
    /* tte_write("Amogus!!"); */
}

void updateGame(){
    if(state != 1){
        return;
    }

    vu32 dx = 0, dy = 0;

    /* dx = key_tri_horz(); */
    /* dy = key_tri_vert(); */

    if(key_is_down(KEY_UP) && am.y > 0){
        dy = -1;
    }else if(key_is_down(KEY_DOWN) && am.y + am.h < MAP_HEIGHT * 8){
        dy = 1;
    }

    if(key_is_down(KEY_LEFT) && am.x > 0){
        dx = -1;
    }else if(key_is_down(KEY_RIGHT) && am.x + am.w < MAP_WIDTH * 8){
        dx = 1;
    }

    updateAmogus(&am, dx, dy);
    updateAmogus(&am2, 0, 0);

    if(am.dx != 0 || am.dy != 0){
        animAmogus(&am, 0, 64);
    }else {
        animAmogus(&am, 128, 0);
    }

    animAmogus(&am2, 63, 64);

    // Copy objects to OAM
    oam_copy(oam_mem, obj_buffer, 2);
}

int main(){
    reread:
    mx = ( am.x + 32 / 2 ) - ( 240 / 2 );
    my = ( am.y + 32 / 2 ) - ( 160 / 2 );

    initMenu();
    initGame();

    while(1){
        vid_vsync();
        key_poll();
        tick++;

        if( key_hit(KEY_START) && state == 0 ){
            changeState(1);
            goto reread;
        }

        updateGame();

        /* mx += key_tri_horz(); */
        /* my += key_tri_vert(); */
        
        mx = ( am.x + 32 / 2 ) - ( SCREEN_WIDTH / 2 );
        my = ( am.y + 32 / 2 ) - ( SCREEN_HEIGHT / 2 );

        REG_BG0HOFS = mx;
        REG_BG0VOFS = my;
    }
    
    return 0;
}
