#include <tonc.h>
#include <maxmod.h>

#include "../include/res.h"
#include "../include/void.h"
#include "soundbank.h"
#include "soundbank_bin.h"

using namespace std;

enum States{
    INTRO, MENU, SLIDE, GAME, GAME_OVER
};

int max_slides = 0;

/* #define SLEEP(time) VBlankIntrDelay(time * 60) */
#define loadSlides(index) loadBmp16(slide_##index); max_slides++;

int STATE = INTRO;
int slides_count = 1;

mm_sound_effect get_coin = {
    { SFX_GET_COIN }, 
    (int)(1.0f * (1<<10)),
    0,
    255,
    127,
};

class Ent{
    private:
        void updateAttr(OBJ_ATTR *o){
            int div_sz = 4;
            switch(this->w){
                case 16:
                    div_sz = 2;
                    break;
                case 8:
                    div_sz = 4;
                    break;
            }

            obj_set_attr(o, o->attr0, o->attr1, ( o->attr2 * this->w) / div_sz );

        }

    public:
        int x, y, w, h, obj_id, dx = 0, dy = 0;
        uint id;
        bool flip = false;
        int anims[];

        Ent(int x=0, int y=0, int w=16, int h=16){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }

        void update(int rate=1){

            if( TICK % rate == 0 ){
            if( !this->isSolid() ){
                this->x += this->dx;
                this->y += this->dy;
            }
            }

            this->updateAttr(&obj_buffer[this->obj_id]);
            flipSprite(&obj_buffer[this->obj_id], this->flip);
            setObjectPosition(&obj_buffer[this->obj_id], this->x, this->y);
        }

        void draw(){
            this->obj_id = NUM_OBJS;
            spr(this->id, this->x, this->y, this->flip, SQUARE, 8, this->w);
        }

        void move(int dx, int dy){
            this->dx = dx;
            this->dy = dy;
        }

        bool isSolid(){
            int px = this->x + this->dx, py = this->y + this->dy, pw = this->w - 1, ph = this->h - 1;

            if( 
                    mget( px / 8, py / 8, solids ) == 0x0001 ||
                    mget( ( px + pw ) / 8, py / 8, solids ) == 0x0001 ||
                    mget( px / 8, ( py + ph ) / 8, solids ) == 0x0001 ||
                    mget( ( px + pw ) / 8, ( py + ph ) / 8, solids ) == 0x0001
              ){
                return true;
            }
            
            return false;
        }

        void setId(uint id){
            obj_buffer[this->obj_id].attr2 = id;
        }

};

int TIMER = 60;

Ent player( 7 * 16, 4 * 16 );
uint max_pgcat = 40; 
int p_dir = 0, p_get_coin_anim_t = max_pgcat, p_coins = 0;
bool p_get_coin = false, p_game_over = false;

int game_over_timer = 80;

Ent coin( 8 * 16, 4 * 16 );

int anim_coin[8] = {4, 5, 6, 7, 8, 9, 10, 11};

#define addTextBox(text, f) \
    text_box_on = true; \
    tte_printf("#{es;P:4,116}"); \
    tte_printf(text, f);

void updateCoinAndPlayer(){
    if( player.x == coin.x && player.y == coin.y ){	
        nocash_puts("> Player get a coin <");
        coin.x = 16 * ( qran_range(16, 224) / 16 );
        coin.y = 16 * ( qran_range(16, 144) / 16 );
        p_get_coin = true;
        p_get_coin_anim_t = max_pgcat; 
        ++p_coins;
        mmEffectEx(&get_coin);
    }

    if( p_get_coin_anim_t <= 0 ){
        p_get_coin = false;
    }else {
        player.dx = 0;
        player.dy = 0;
        player.setId(3);
        p_get_coin_anim_t--;
    }

    if( !p_get_coin ){
        if( key_is_down( KEY_UP ) ){
            player.move(0, -16);
            p_dir = 1;
        }else if( key_is_down( KEY_DOWN ) ){
            player.move(0, 16);
            p_dir = 0;
        }else { player.dy = 0; }

        if( key_is_down( KEY_LEFT ) ){
            player.move(-16, 0);
            p_dir = 2;
            player.flip = true;
        }else if( key_is_down( KEY_RIGHT ) ){
            player.move(16, 0);
            p_dir = 2;
            player.flip = false;
        }else { player.dx = 0; }

        player.setId(p_dir);
    }
    coin.setId(anim_coin[ ( ( TICK / 12 ) % 4 )]);

    player.update(10);
    coin.update();
}

void initGameOver(){
    loadBmp16(game_over);
    setMode( DCNT_MODE3 | DCNT_BG2 );
    tte_init_bmp_default(3);

    tte_write("#{P:72, 146}");
    tte_write("-- Obrigado por jogar --");
}

int intro_imgs = 0;
void initIntro(){
    switch(intro_imgs){
        case 0:
            loadBmp16(intro_msg);
            break;
        case 1:
            loadBmp16(intro_logo);
            break;
    }

    setMode( DCNT_MODE3 | DCNT_BG2 );
}

void initMenu(){
    loadBmp16(title);
    setMode( DCNT_MODE3 | DCNT_BG2 );
    tte_init_bmp_default(3);

    tte_write("#{P:104, 130}");
    tte_write("Play");
}

void initSlide(){
    switch(slides_count){
        case 1:
            loadSlides(1);
            nocash_puts("slide [1]");
            break;
        case 2:
            loadSlides(2);
            nocash_puts("slide [2]");
            break;
        case 3:
            loadSlides(3);
            nocash_puts("slide [3]");
            break;
        case 4:
            loadSlides(4);
            nocash_puts("slide [4]");
            break;
        case 5:
            loadSlides(5);
            nocash_puts("slide [5]");
            break;
        case 6:
            loadSlides(6);
            nocash_puts("slide [6]");
            break;
    }

    setMode( DCNT_MODE3 | DCNT_BG2 );
}

void initGame(){
    // Reset all graphics
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_OAM);

    // Load all graphics
    loadSprite8(spr_player, 0);
    loadSprite8(spr_coin, 16);
    loadTileset8(tileset, 0, 0);
    loadMap(bg0, MAP1_LENGTH, 31, 0);
    loadMap(bg1, MAP1_LENGTH, 30, 0);

    initOam();

    // Enable mode and background
    REG_BG0CNT = BG_CBB( 0 ) | BG_SBB( 31 ) | BG_8BPP | BG_REG_32x32 | BG_PRIO(1);
    REG_BG1CNT = BG_CBB( 0 ) | BG_SBB( 30 ) | BG_8BPP | BG_REG_32x32 | BG_PRIO(2);
    setMode( DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D );

    player.id = 0;
    coin.id = 4;

    tte_init_chr4c_b4_default( 2, BG_CBB(1) | BG_SBB( 28 ) | BG_PRIO(0) );
    tte_init_con();

    player.draw();
    coin.draw();
}

int bg_x = 0, bg_y = 0;
void updateGame(){
    if( TICK % 64 == 0 ){
        TIMER--;
    }else if( TIMER <= 0 ){
        p_game_over = true;
    }

    bg_x++;
    bg_y++;

    if( !p_game_over ){
        tte_printf("#{es;P:2, 148}Timer: %i", TIMER);
        tte_printf("#{P:100, 148}Coins: ? %i ?", p_coins);
    }else {
        game_over_timer--;
        tte_printf("#{es;P:90, 68}Game Over");
        return;
    }

    moveBg(1, bg_x, bg_y);
    updateCoinAndPlayer();
}

void resetGame(){
    TICK = 0;
    p_game_over = false;
    STATE = INTRO;
    TIMER = 60;
    p_coins = 0;
    game_over_timer = 80;
    intro_imgs = 0;
    slides_count = 1;
}

int main(){
    irq_init(NULL);
    irq_add(II_VBLANK, mmVBlank);
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

    refresh:
    if( STATE == INTRO ){
        initIntro();
    }else if( STATE == MENU ){
        mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );
        mmSetModuleVolume(70);
        initMenu();
    }else if( STATE == SLIDE ){
        initSlide();
        mmStop();
    }else if( STATE == GAME ){
        mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );
        mmSetModuleVolume(50);
        initGame();
    }else if( STATE == GAME_OVER ){
        mmStop();
        initGameOver();
    }

    while(true){
        mmFrame();
        VBlankIntrWait();
        key_poll();
        TICK++;

        if( STATE == INTRO ){
            if( TICK % 130 == 0 ){
                intro_imgs++;
                goto refresh;
            }

            if( intro_imgs > 1 ){
                STATE = MENU;
                goto refresh;
            }
        }else if( STATE == MENU ){
            if( key_hit( KEY_ANY ) ){
                STATE = SLIDE;
                goto refresh;    
            }

        }else if( STATE == SLIDE ){
            if( key_hit( KEY_A ) ){ 
                slides_count++;
                goto refresh; 
            }else if( slides_count > max_slides ){
                STATE = GAME;
                goto refresh;
            }
        }else if( STATE == GAME ){
            updateGame();
            
            if( game_over_timer <= 0 ){
                RegisterRamReset(RESET_VRAM);
                STATE = GAME_OVER;
                goto refresh;
            }
        }else if( STATE == GAME_OVER ){
            if( key_hit( KEY_A ) ){
                resetGame();
                goto refresh;
            }
        }

        loadObjectsToOam();
    }
    return 0;
}
