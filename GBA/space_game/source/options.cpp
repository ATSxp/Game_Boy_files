#include "../include/options.h"

u16 LANGUAGE = LANGUAGE_PTBR;

int cursor_x, cursor_y;
u16 gui_cursor = 0, cursor_space;
BOOL cursor_center;

typedef struct {
    u16 slot, tid;
    int x, y;
}c_spr;

c_spr cursor_spr[4] = {
    { 0, 0 },
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
};

void initGUI( int x, int y, u16 space, BOOL center){
    loadTileObj(spr_p_item_slot, 16);

    cursor_x = x;
    cursor_y = y;
    cursor_space = space;
    cursor_center = center;

    SPRITE_TOTAL_OAM += 4;
}

void updateGUI( ButtonGUI btns[], u16 total_btns ){
    vs32 n_len;

    if( key_hit( KEY_UP ) && gui_cursor > 0 ){
        gui_cursor--;
    }else if( key_hit( KEY_DOWN ) && gui_cursor < total_btns - 1 ){
        gui_cursor++;
    }

    nocash_puts_str( to_string( gui_cursor ) );

    tte_erase_screen();
    for( int ii = 0; ii < total_btns; ii++ ){
        if( gui_cursor == ii ){
            btns[ii].c = 8;
        }else { btns[ii].c = 7; }

        string cmd = "#{P:-32,-32;ci:"+ to_string( btns[ii].c ) +"}";
        string n = cmd + btns[ii].name;

        n_len = tte_write( n.c_str() );

        if( n_len < 18 ){ btns[ii].w = ( n_len - 17 ) * 5; }else { btns[ii].w = ( n_len - 18 ) * 5; }
        btns[ii].h = 8;

        if( cursor_center ){
            cursor_x = ( SCREEN_WIDTH - btns[ii].w ) >> 1;
        }

        btns[ii].x = cursor_x;
        btns[ii].y = cursor_space * ii + cursor_y;

        tte_set_pos( btns[ii].x, btns[ii].y );
        tte_write_str( btns[ii].name );
    }

    for( int jj = 0; jj < 4; jj++ ){
        // Topo esquerdo
        cursor_spr[0].x = btns[ gui_cursor ].x - 4;
        cursor_spr[0].y = btns[ gui_cursor ].y - 2;

        // Topo direito
        cursor_spr[1].x = btns[ gui_cursor ].x + btns[ gui_cursor ].w;
        cursor_spr[1].y = btns[ gui_cursor ].y - 2;

        // Baixo esquerdo
        cursor_spr[2].x = btns[ gui_cursor ].x - 4;
        cursor_spr[2].y = btns[ gui_cursor ].y + 6;

        // Baixo direito
        cursor_spr[3].x = btns[ gui_cursor ].x + btns[ gui_cursor ].w;
        cursor_spr[3].y = btns[ gui_cursor ].y + 6;

        obj_set_attr( &OBJ_BUFFER[ cursor_spr[jj].slot ],  ATTR0_4BPP | ATTR0_SQUARE, ATTR1_SIZE_8x8, ATTR2_BUILD(cursor_spr[jj].tid, 0, 0) );
        obj_set_pos( &OBJ_BUFFER[ cursor_spr[jj].slot ], cursor_spr[jj].x, cursor_spr[jj].y);
    }

    if( key_hit( KEY_A ) ){
        btns[ gui_cursor ].action();
    }
}
