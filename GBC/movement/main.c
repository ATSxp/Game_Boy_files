#include <gb/gb.h>
#include <gb/drawing.h>
#include <stdio.h>
#include "assets/maps/tileset.c"
#include "assets/maps/map1.c"
#include "assets/maps/map2.c"
#include "assets/sprites/player_sprites.c"
#include "src/player.c"

void Load(){
    set_bkg_data(0, 7, tileset);
    playerLoad();

    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

}

void Update(){
        // if(joypad() == J_B){
        //     set_bkg_tiles(0, 0, map2Width, map2Height, map2); // map 2
        // }else{
        // }
    set_bkg_tiles(0, 0, map1Width, map1Height, map1); // map 1
    playerUpdate();

    delay(70);
}

void Draw(){
    playerDraw();
}

void main(){
    Load();

    while(1){
        Update();
        Draw();

    }

}