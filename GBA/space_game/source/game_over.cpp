#include "../include/game_over.h"

extern int spc0_y;
extern BOOL ready;
u16 gmo_timer = MAX_GAME_OVER_TIMER;
ButtonGUI game_over_btns[2];

void initGameOver(){
    game_over_btns[0] = { translTxt( TRY_AGAIN ), _tryAgain, ENvsPTBR(-8, 2) };
    game_over_btns[1] = { translTxt( BACK_TO_MAIN_MENU ), _backToMenu, ENvsPTBR(-10, -8) };

    initGUI(61, game_over_btns, 2, 2, SCREEN_HEIGHT - 64, 12, {7, 8}, FALSE);
}

void updateGameOver(){
    obj_hide(&OBJ_BUFFER[ 23 ]);

    if( gmo_timer <= 0 ){
        for(u16 i = 56; i < 61; i++){
            obj_hide(&OBJ_BUFFER[i]);
        }

        updateGUI();

        string _game_over_txt = translTxt(GAME_OVER);
        tte_set_pos( ( SCREEN_WIDTH - ( _game_over_txt.length() * 5 ) ) >> 1 , 74);
        tte_write_str( "#{ci:7}" + _game_over_txt );

    }else {
        gmo_timer--;
    }

}

void resetGame(){
    tte_erase_screen();

    spc0_y = 0;
    ready = FALSE;
    p_points = 0;
    player.dx = 0;
    player.dy = 0;
    player.hp = MAX_HP_PLAYER;
    player.dead = FALSE;
    player.pos.x = ( SCREEN_WIDTH - 16 ) / 2;
    player.pos.y = SCREEN_HEIGHT;

    gmo_timer = MAX_GAME_OVER_TIMER;
    /* resetGUI(); */
    resetVoid();
}

void _tryAgain(){ resetGame(); setScene(game_scene); }
void _backToMenu(){ resetGame(); setScene(menu_scene); }
