#include "../include/menu.h"

void actionPlay(){
    setScene(game_scene);
}

cu16 NUM_BTNS = 3;
ButtonGUI menu_btns[NUM_BTNS];

void initMenu(){
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ_1D | DCNT_OBJ;

    loadPalObj(spr_menu_cursor);
    memcpy16(pal_bg_bank[15], spr_menu_cursor_pal_bin, spr_menu_cursor_pal_bin_size / 2);

    tte_init_chr4c(
            1, 
            BG_CBB(1) | BG_SBB(31) | BG_PRIO(1), 
            0xF000, 
            0x0201,
            0x28A3 | 0x288F << 16, 
            &verdana9Font, 
            (fnDrawg)chr4c_drawg_b1cts_fast);

    initVoid();

    menu_btns[0] = { translTxt( NEW_GAME ), actionPlay };
    menu_btns[1] = { translTxt( LOAD_GAME ), actionPlay };
    menu_btns[2] = { translTxt( OPTIONS ), actionPlay };

    initGUI(0, menu_btns, NUM_BTNS, 2, 80, 12, {7, 8}, FALSE);

    REG_BG_OFS[1].y = 0;
}

void updateMenu(){
    updateGUI();

    tte_write_str( "#{ci:7;P:"+ to_string( ( SCREEN_WIDTH - 80 ) >> 1 ) +",30}Menu muito fodão" );

    updateVoid();
}

void endMenu(){
    tte_erase_screen();
    resetGUI();
    resetVoid();
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_OAM);
}

Scene menu_scene = {
    initMenu, 
    endMenu,
    updateMenu,
};
