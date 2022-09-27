#include "tonc.h"
#include <string.h>

#include "../include/resources.h" // Assets
#include "../include/void.h" // Engine

int state = 0;
Entity player;

void initMenu(){
    if( state != 0 ){
        return;
    }
    nocash_puts("STATE: [MENU]");

    loadBmp16(bayonetta);
    setMode( DCNT_MODE3 | DCNT_BG2 );

}

void initGame(){
    if( state != 1 ){
        return;
    }
    nocash_puts("STATE: [GAME]");

    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_OAM);

    loadSprite(spr_plr, 0);
    loadTileset(tileset, 0, 0);
    loadMap(tm, MAP_01_LENGTH, 31, 0);
    loadMap(solids, MAP_01_LENGTH, 30, 0);

    REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_REG_32x32;
    REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_REG_32x32;
    setMode( DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D );
 
    initOam();

    initEnt(&player, 16, 16);
    sprEnt(&player, 0, false, SQUARE, 4, 16);
}

int bg_x = 0;
void updateGame(){
    if( state != 1 ){
        return;
    }

    setAnim(&player, 0, 4, 32);

    player.dx = 0;
    player.dy = 0;

    if( key_is_down( KEY_UP ) ){
        player.dy = -1;
    }else if( key_is_down( KEY_DOWN ) ){
        player.dy = 1;
    }

    if( key_is_down( KEY_LEFT ) ){
        player.dx = -1;
    }else if( key_is_down( KEY_RIGHT ) ){
        player.dx = 1;
    }


    if( TICK % 4 == 0 ){
        bg_x++;
    }
    moveBg(1, bg_x, 0);

    updateEnt(&player);

    loadObjectsToOam();
}

int main(){
    refresh:

    initMenu();
    initGame();

    while(1){
        initVoid();

        if( state == 0 ){
            if( key_hit( KEY_START ) ){
                state = 1;
                goto refresh;
            }
        }

        updateGame();
    }
    return 0;
}
