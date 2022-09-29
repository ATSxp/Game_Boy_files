#include "tonc.h"
#include <cstdlib>

#include "../include/res.h"
#include "../include/void.h"

enum States{
    MENU, GAME
};

#define STATE GAME

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

Ent player( 16, 16);
int p_dir = 0;

Ent coin( 32, 16);

int anim_coin[8] = {4, 5, 6, 7, 8, 9, 10, 11};

void updateCoinAndPlayer(){
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

    if( player.x == coin.x && player.y == coin.y ){
            int rx = ( rand() % 240 ) / 16;
            int ry = ( rand() % 160 ) / 16;

            if( rx > 1 && rx < 14 ){
                coin.x = 16 * rx;
            }

            if( ry > 1 && ry < 9 ){
                coin.y = 16 * ry;
            }
    }

    player.update(10);
    coin.update();
}

void initGame(){
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_OAM);

    loadSprite8(spr_player, 0);
    loadSprite8(spr_coin, 16);
    loadTileset8(tileset, 0, 0);
    loadMap(bg0, MAP1_LENGTH, 30, 0);

    initOam();

    REG_BG0CNT = BG_CBB( 0 ) | BG_SBB( 30 ) | BG_8BPP | BG_REG_32x32;
    setMode( DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D );

    player.id = 0;
    coin.id = 4;

    player.draw();
    coin.draw();
}

void updateGame(){
    updateCoinAndPlayer();
}

int main(){

    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    if( STATE == MENU ){
    
    }else if( STATE == GAME ){
        initGame();
    }

    while(true){
        VBlankIntrWait();
        key_poll();
        TICK++;

        if( STATE == MENU ){

        }else if( STATE == GAME ){
            updateGame();
        }

        loadObjectsToOam();
    }
    return 0;
}
