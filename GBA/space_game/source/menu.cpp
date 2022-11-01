#include "../include/menu.h"

void actionPlay(){
    setScene(game_scene);
}

ButtonGUI btns[] = {
    {
        "Play",
        actionPlay
    },
    {
        "Options",
        actionPlay
    },
};
u16 NUM_BTNS = 2;

void initMenu(){
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ_1D | DCNT_OBJ;

    loadPalObj(spr_p_item_slot);

    memcpy16(pal_bg_bank[15], spr_p_item_slotPal, spr_p_item_slotPalLen / 2);
    tte_init_chr4c(
            1, 
            BG_CBB(1) | BG_SBB(31) | BG_PRIO(1), 
            0xF000, 
            0x0201,
            0x28A3 | 0x288F << 16, 
            &verdana9Font, 
            (fnDrawg)chr4c_drawg_b1cts_fast);

    initVoid();
    initGUI(0, 80, 12, TRUE);

}

void updateMenu(){
    updateGUI(btns, NUM_BTNS);
    tte_write_str( "#{ci:7;P:"+ to_string( ( SCREEN_WIDTH - 80 ) >> 1 ) +",30}Menu muito fodão" );
    updateVoid();
}

void endMenu(){
    tte_erase_screen();
    resetVoid();
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_OAM);
}

Scene menu_scene = {
    initMenu, 
    endMenu,
    updateMenu,
};
