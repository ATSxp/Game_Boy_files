#include "tonc.h"
#include <cstdlib>
#include <stdlib.h>

#include "../include/res.h"
#include "../include/void.h"

enum States{
    MENU, SLIDE, GAME
};

int max_slides = 0;

#define SLEEP(time) VBlankIntrDelay(time * 60)
#define loadSlides(index) loadBmp16(slide_##index); max_slides++;

int STATE = MENU;
int slides_count = 1;

// Get a tile id on map
u8 mget(int x, int y, cu16 *bg){
    return bg[y * MAP1_WIDTH + x];
}

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

Ent player( 7 * 16, 4 * 16 );
int p_dir = 0;
bool p_get_coin = false;
uint max_pgcat = 40; 
int p_get_coin_anim_t = max_pgcat; 

Ent coin( 8 * 16, 4 * 16 );

int anim_coin[8] = {4, 5, 6, 7, 8, 9, 10, 11};

void updateCoinAndPlayer(){
    if( player.x == coin.x && player.y == coin.y ){	
        nocash_puts("> Player get a coin <");
        coin.x = 16 * ( qran_range(16, 224) / 16 );
        coin.y = 16 * ( qran_range(16, 144) / 16 );
        p_get_coin = true;
        p_get_coin_anim_t = max_pgcat; 
    }

    if( p_get_coin_anim_t <= 0 ){
        p_get_coin = false;
    }else {
        player.dx = 0;
        player.dy = 0;
        p_get_coin_anim_t--;
    }

    if( p_get_coin ){
        player.setId(3);
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

    coin.setId(anim_coin[ ( ( TICK / 12 ) % 4 )]);
    player.setId(p_dir);

    }

    player.update(10);
    coin.update();
}

void winCopy(){
    REG_WIN0H = ( player.x - player.w ) << 8 | ( ( player.x - player.w ) + 48 );
    REG_WIN0V = ( player.y - player.h ) << 8 | ( ( player.y - player.h ) + 48 );
}

void initMenu(){
    loadBmp16(title);

    setMode( DCNT_MODE3 | DCNT_BG2 );
    tte_init_bmp_default(3);

    tte_write("#{P:104, 130}");
    tte_write("Play");
}

void nextSlide(){
    slides_count++;
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_PALETTE);
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
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_OAM);

    // Load all graphics
    loadSprite8(spr_player, 0);
    loadSprite8(spr_coin, 16);
    loadTileset8(tileset, 0, 0);
    loadMap(bg0, MAP1_LENGTH, 30, 0);
    loadMap(bg1, MAP1_LENGTH, 20, 0);

    initOam();

    // Enable mode and background
    REG_BG0CNT = BG_CBB( 0 ) | BG_SBB( 30 ) | BG_8BPP | BG_REG_32x32;
    REG_BG1CNT = BG_CBB( 0 ) | BG_SBB( 20 ) | BG_8BPP | BG_REG_32x32;
    setMode( DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_WIN0 | DCNT_WIN1);

    REG_WININ = WININ_BUILD(WIN_BG0 | WIN_OBJ, WIN_BG1);
    winCopy();
    REG_WIN1H = 0 | 240;
    REG_WIN1V = 0 | 160;

    player.id = 0;
    coin.id = 4;

    player.draw();
    coin.draw();
}

int bg_x = 0, bg_y = 0;
void updateGame(){
    updateCoinAndPlayer();
    winCopy();

    bg_x++;
    bg_y++;

    REG_BG1HOFS = -bg_x;
    REG_BG1VOFS = -bg_y;
}

int main(){
    refresh:

    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    if( STATE == MENU ){
        initMenu();
    }else if( STATE == SLIDE ){
        initSlide();

    }else if( STATE == GAME ){
        initGame();
    }

    while(true){
        VBlankIntrWait();
        key_poll();
        TICK++;

        if( STATE == MENU ){
            if( key_hit( KEY_ANY ) ){
                STATE = SLIDE;
                goto refresh;    
            }
        }else if( STATE == SLIDE ){
            if( key_hit( KEY_A ) ){ 
                nextSlide();
                goto refresh; 
            }else if( slides_count > max_slides ){
                STATE = GAME;
                goto refresh;
            }
        }else if( STATE == GAME ){
            updateGame();
        }

        loadObjectsToOam();
    }
    return 0;
}
