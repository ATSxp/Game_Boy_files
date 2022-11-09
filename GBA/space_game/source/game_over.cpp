#include "../include/game_over.h"
#include "../include/demo_game_over.h"

extern u32 world_seconds;
extern int spc0_y;
extern BOOL ready;
u16 gmo_timer = MAX_GAME_OVER_TIMER;
ButtonGUI game_over_btns[2];

BOOL _make_fade;

void initGameOver(){
    game_over_btns[0] = { translTxt( TRY_AGAIN ), _tryAgain, ENvsPTBR(-8, 2) };
    game_over_btns[1] = { translTxt( BACK_TO_MAIN_MENU ), _backToMenu, ENvsPTBR(-10, -8) };

    initGUI(61, game_over_btns, 2, 2, SCREEN_HEIGHT - 64, 12, {7, 8}, FALSE);

    _make_fade = FALSE;
}

void updateGameOver(){
    // delete/hide all bullets
    pb.clear();
    for( u16 j = 1; j < 15; j++ ) obj_hide( &OBJ_BUFFER[j] );

    obj_hide(&OBJ_BUFFER[ 23 ]);

    if( gmo_timer <= 0 ){
        if( !_make_fade ){
            _make_fade = TRUE;
            tte_erase_screen();
            fadeIn(2);
        }

        for( u16 i = 56; i < 61; i++ ) obj_hide( &OBJ_BUFFER[i] ); // Items hud
        for( u16 k = 24; k < 32; k++ ) obj_hide( &OBJ_BUFFER[k] ); // Life hud

        /* updateGUI(); */
        
        /* string _game_over_txt = translTxt(GAME_OVER); */
        /* tte_set_pos( ( SCREEN_WIDTH - ( _game_over_txt.length() * 5 ) ) >> 1 , 74); */
        /* tte_write_str( "#{ci:7}" + _game_over_txt ); */

        // ! DEMO !
        if( !fade_in_on ){
            setScene(demo_game_over_scene); 
            resetGame(); 
        }
    }else {
        gmo_timer--;
    }

}

void resetGame(){
    tte_erase_screen();
    sqran(world_seconds);

    spc0_y = 0;
    ready = FALSE;
    p_points = 0;
    p_imortal = FALSE;
    p_imortal_item = 0;
    p_potions = 0;
    p_boost_bullets = 0;
    p_mega_bullets = 0;
    p_multi_bullets = 0;
    player.dx = 0;
    player.dy = 0;
    player.hp = MAX_HP_PLAYER;
    player.dead = FALSE;
    player.pos.x = ( SCREEN_WIDTH - 16 ) / 2;
    player.pos.y = SCREEN_HEIGHT;

    gmo_timer = MAX_GAME_OVER_TIMER;
    resetGUI();
    resetVoid();
}

void _tryAgain(){ resetGame(); setScene(game_scene); }
void _backToMenu(){ 
    resetGame();
    setScene(demo_menu_scene);
    //setScene(menu_scene); 
}
