#include <gb/gb.h>
#include <stdio.h>
#include "bkg_tileset.c"
#include "bkg_map.c"
#include "dummy.c"

/*
    NOTES:
    SCREEN W = 160;
    SCREEN H = 152
*/

UINT8 t = 0; // tick
UINT8 x = (160 - 8) / 2, y = 135; // X and Y axis of sprite

void main(){    
    UINT8 index = 1;
    set_bkg_data(0, 8, bkg_tileset);
    set_bkg_tiles(0, 0, 40, 18, bkg_map);
    set_sprite_data(0, 3, dummy_sprite);
    set_sprite_tile(0, index);
    move_sprite(0, x, y);

    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    while(1){
        set_sprite_tile(0, index);
        scroll_sprite(0, - 1, 0);
        scroll_bkg(1, 0);
        
        if(index){
            if(index == 1 && t % 4 == 0){
                index = 2;
            }else if(index == 2 && t % 4 == 0){
                index = 1;
            }
        }

        delay(100);
        t++;
    }
}