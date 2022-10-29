#include "../include/game_over.h"

u16 gmo_timer = MAX_GAME_OVER_TIMER;

void updateGameOver(){
    gmo_timer--;

    if( gmo_timer <=0 ){
        tte_erase_screen();
        tte_write_str("#{ci:7;P:97,76}Game Over");

        /* if( key_hit( KEY_START ) ){ */
        /*     r_main = TRUE; */
        /* } */
    }
}
