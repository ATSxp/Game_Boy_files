#ifndef SHIP_H
#define SHIP_H

#include "void.h"

class Ship{
    private:
        u16 timer_to_damage = 5;
    public:
        Axis pos;
        Size size = { 16, 16 };
        Sprite sp;
        u16 hp;
        u8 dead = FALSE, damaged = FALSE;
        int dx = 0, dy = 0, spd = 1;

        Ship( int x = 0, int y = 0 , u16 hp = 1 );
        void update();
        void setDamage( s16 dmg );
        u8 shipVsShip( Ship *s );
};

#endif // SHIP_H
