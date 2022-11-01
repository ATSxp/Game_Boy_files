#include "../include/game_over.h"

extern BOOL ready;
u16 gmo_timer = MAX_GAME_OVER_TIMER;

void updateGameOver(){
    obj_hide(&OBJ_BUFFER[ 23 ]);

    if( gmo_timer <= 0 ){
        for(u16 i = 56; i < 61; i++){
            obj_hide(&OBJ_BUFFER[i]);
        }

        tte_erase_screen();
        tte_write_str( translTxt(GAME_OVER) );

        if( key_hit( KEY_START ) ){ resetGame(); }
    }else {
        gmo_timer--;
    }
}

void resetGame(){
    resetVoid();
    tte_erase_screen();

    ready = FALSE;
    player.dx = 0;
    player.dy = 0;
    player.hp = MAX_HP_PLAYER;
    player.dead = FALSE;
    player.pos.x = ( SCREEN_WIDTH - 16 ) / 2;
    player.pos.y = SCREEN_HEIGHT;

    gmo_timer = MAX_GAME_OVER_TIMER;
    setScene(game_scene);
}
