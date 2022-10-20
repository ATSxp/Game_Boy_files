#ifndef SPRITES_H
#define SPRITES_H

#include <tonc.h>
#include <math.h>
#include "load.h"

#define MAX_SPRITES_IN_OAM 128

extern u16 SPRITE_TOTAL_OAM;
extern OBJ_ATTR OBJ_BUFFER[ MAX_SPRITES_IN_OAM ];

class Sprite {
    private:
        OBJ_ATTR *spr;
        u16 slot_in_oam;
    public:
        Axis pos = { 0, 0 };
        Size size;
        u16 tid = 0, pal = 0, prio = 0, tick = 0;
        u8 flip_h = FALSE, flip_v = FALSE;

        Sprite( s16 slot = -1 );
        void setAttr( u16 attr0, u16 attr1 );
        void setPos( int nx = 0, int ny = 0 );
        Axis getPos();
        Size getSize();
        int getWidth();
        int getHeight();
        void update();
        void hide();
        void unhide();
        void anim(u16 start, u16 frames, u16 rate);
        // não funciona corretamente
        /* void animForward( u16 frames, u16 rate ); */
};

void initOam();
void updateOam();
u8 checkSpriteIsHided( u16 id );

#endif // SPRITES_H
