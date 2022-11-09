#include "../include/demo_game_over.h"

#include "../include/demo_menu.h"
#include "../include/texts_ptbr.h"

#include "maps/pal_game_obj.h"
#include "maps/pal_game_bg.h"
#include "demo_game_over_img_bin.h"
#include "demo_game_over_pal_bin.h"

void initDemoGameOver(){
    REG_DISPCNT = DCNT_MODE4 | DCNT_BG2;

    loadPalBg(demo_game_over);
    loadBmp(demo_game_over);

    tte_init_bmp(4, &verdana9Font, NULL);
    memcpy16(pal_bg_bank[15], pal_gamePal, pal_gamePalLen / 2);

    tte_set_margins(0, 8, 240, 160 - 8);

    tte_write_str( translTxt(GAME_OVER_MSG_DEMO) );

    string _ptr = translTxt(PRESS_TO_RESET_DEMO);

    tte_set_pos( ( SCREEN_WIDTH - ( ( _ptr.length() - 9 ) * 5 ) ) >> 1, 144 );
    tte_write_str( _ptr );
}


void updateDemoGameOver(){
    if( key_hit( KEY_ANY ) ) setScene(demo_menu_scene);
}


void endDemoGameOver(){
    tte_erase_screen();
    resetVoid();
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_PALETTE);

    REG_BG_OFS[2].x = 0;
    REG_BG_OFS[2].y = 0;
}

Scene demo_game_over_scene = {
    initDemoGameOver,
    endDemoGameOver,
    updateDemoGameOver
};
