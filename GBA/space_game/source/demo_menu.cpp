#include "../include/demo_menu.h"

extern u32 world_seconds;
vu16 tte_delay, tte_len_tm, tm_to_start;
vu32 tte_tick, spc_dx, spc_dy;
BOOL is_start;

void demoMenuInit(){
    sqran(world_seconds);

    loadPalBg(space_demo_menu);
    loadTile(space_demo_menu);
    loadTile(demo_menu);

    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2;

    Map spc1(2, BG_CBB(0) | BG_SBB(27) | BG_4BPP, space_demo_menu_map_bin, SPACE_WIDTH, SPACE_HEIGHT);
    Map title(1, BG_CBB(0) | BG_SBB(29) | BG_4BPP, demo_menu_map_bin, SPACE_WIDTH, SPACE_HEIGHT);

    memcpy16(pal_bg_bank[15], tileset_space_pal_bin, tileset_space_pal_bin_size / 2);
    tte_init_chr4c(
            0, 
            BG_CBB(2) | BG_SBB(31), 
            0xF000, 
            0x0201,
            0x28A3 | 0x288F << 16, 
            &verdana9Font, 
            (fnDrawg)chr4c_drawg_b1cts_fast);

    initVoid();

    tte_delay = 8;
    tte_len_tm = 4;
    tte_tick = 0;
    tm_to_start = 50;
    is_start = FALSE;

    spc_dx = 0;
    spc_dy = 0;

    REG_BG_OFS[2].x = 0;
    REG_BG_OFS[2].y = 0;

    tte_set_pos(0, 144);
    tte_write_str("#{ci:7}< #{ci:8}@#{ci:7}ATSxp >");
}

void demoMenuUpdate(){
    tte_tick++;

    string _str = translTxt( START_GAME_DEMO );
    vu16 _len = _str.length() * 5;
    vu32 _x = ( SCREEN_WIDTH - _len ) >> 1;

    tte_set_pos( _x, 88 );

    if( ( tte_tick / tte_delay ) % tte_len_tm == 0 ){
        tte_erase_rect(11 * 8, 88, _len * 4, 16 * 8);
    }else {
        tte_write_str( "#{ci:7}" + _str );
    }

    if( key_hit( KEY_ACCEPT ) && !is_start ){
        is_start = TRUE;
    }else if( is_start ) {
        tte_delay = 4;
        tte_len_tm = 2;
        tm_to_start--;
    }

    if( tm_to_start <= 0 ) setScene(game_scene);

    spc_dx++;
    spc_dy++;

    REG_BG_OFS[2].x = spc_dx;
    REG_BG_OFS[2].y = spc_dy;
    updateVoid();
}

void demoMenuEnd(){
    REG_BG_OFS[2].x = 0;
    REG_BG_OFS[2].y = 0;

    resetVoid();
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_OAM);
    RegisterRamReset(RESET_VRAM);
}

Scene demo_menu_scene = {
    demoMenuInit,
    demoMenuEnd,
    demoMenuUpdate
};
