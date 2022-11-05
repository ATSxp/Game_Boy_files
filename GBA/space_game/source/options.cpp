#include "../include/options.h"

u16 LANGUAGE = LANGUAGE_EN;

int cursor_x, cursor_y;
u16 gui_cursor = 0, cursor_space;
BOOL cursor_center;

Sprite cursor_spr;
ButtonGUI *btns;
u16 _total_btns, _clrs[2];
vu16 _tid;

void initGUI( u16 slot_obj, ButtonGUI *btn_list, cu16 total_btns, int x, int y, u16 space, vector<u16> clrs, BOOL center){
    btns = btn_list;
    _total_btns = total_btns;
    _clrs[0] = clrs[0];
    _clrs[1] = clrs[1];

    loadTileObj(spr_menu_cursor, 8);
    _tid = SPRITE_IN_VRAM_OBJ - 4;

    cursor_spr.newSprite(slot_obj);
    cursor_spr.flip_h = TRUE;

    cursor_x = x;
    cursor_y = y;
    cursor_space = space;
    cursor_center = center;

    SPRITE_TOTAL_OAM += 1;
}

void updateGUI(){
    tte_erase_screen();

    if( key_hit( KEY_UP ) && gui_cursor > 0 ){
        gui_cursor--;
    }else if( key_hit( KEY_DOWN ) && gui_cursor < _total_btns - 1 ){
        gui_cursor++;
    }

    for( int ii = 0; ii < _total_btns; ii++ ){
        if( gui_cursor == ii ){
            btns[ii].c = _clrs[1];
        }else { btns[ii].c = _clrs[0]; }

        string _cmd = "#{ci:"+ to_string( btns[ii].c ) +"}";
        tte_cmd_default(_cmd.c_str());

        btns[ii].w = btns[ii].name.length() * ( LANGUAGE == LANGUAGE_EN ? 6 : 5 );
        btns[ii].h = 8;

        if( cursor_center ){
            cursor_x = ( SCREEN_WIDTH - btns[ii].w ) >> 1;
        }

        btns[ii].x = cursor_x;
        btns[ii].y = cursor_space * ii + cursor_y;

        tte_set_pos( btns[ii].x, btns[ii].y );
        tte_write_str( "#{ci:" + to_string(btns[ii].c) + "}" + btns[ii].name );
    }

    cursor_spr.setAttr( ATTR0_4BPP | ATTR0_SQUARE, ATTR1_SIZE_8x8 );
    cursor_spr.setPos( ( btns[ gui_cursor ].x + btns[ gui_cursor ].w ) + btns[ gui_cursor ].offset, btns[ gui_cursor ].y + 2 );
    cursor_spr.anim(_tid, 4, 12);

    cursor_spr.update();

    if( key_hit( KEY_A ) && btns[ gui_cursor ].action != NULL ){
        btns[ gui_cursor ].action();
    }
}

void resetGUI(){
    gui_cursor = 0;
}
