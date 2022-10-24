#include "../include/game.h"

Map spc0(0, BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_PRIO(2), space_bg0, 32, 64);
Map spc1(1, BG_CBB(0) | BG_SBB(25) | BG_4BPP | BG_PRIO(1), space_bg1, 32, 64);
int spc0_y = 0;
Sprite hud(18);

void initGame(){
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ_1D | DCNT_OBJ;

    tte_init_se_default(2, BG_CBB(1) | BG_SBB(20) | BG_PRIO(0) );
    tte_init_con();

    loadPalBg(tileset_space);
    loadTile(tileset_space);

    initVoid();
    initPlayer();
    initEnemy();
    initHudPlayer();
}

void updateGame(){
    updateHudPlayer();
    updatePlayer();
    updateEnemies();

    spc0_y--;
    REG_BG_OFS[0].y = spc0_y;
    REG_BG_OFS[1].y = spc0_y * 3;

    updateVoid();
}

void initHudPlayer(){
    loadTileObj(spr_hp_player, 8);
}

void updateHudPlayer(){
    for( int i = 0; i < MAX_HP_PLAYER; i++ ){
        OBJ_ATTR *spr = &OBJ_BUFFER[ ( 16 + MAX_HP_PLAYER ) + i ];
        obj_set_attr( spr, 
                ATTR0_4BPP | ATTR0_SHAPE(0), 
                ATTR1_SIZE_8, 
                ATTR2_BUILD(65, 0, 0) );
        obj_set_pos( spr, i * 9 + 2, SCREEN_HEIGHT - 12 );
    }

    for( int i = 0; i < player.hp; i++ ){
        OBJ_ATTR *spr = &OBJ_BUFFER[ 16 + i ];
        obj_set_attr( spr, 
                ATTR0_4BPP | ATTR0_SHAPE(0), 
                ATTR1_SIZE_8, 
                ATTR2_BUILD(64, 0, 0) );
        obj_set_pos( spr, -( i - 1 ) * 9 + ( 8 * 7 ), SCREEN_HEIGHT - 12 );

    }

    obj_hide_multi( &OBJ_BUFFER[ 16 ], ABS( player.hp - MAX_HP_PLAYER ) );
}
