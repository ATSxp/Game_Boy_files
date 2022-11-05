#ifndef SHIP_H
#define SHIP_H

#include "void.h"

#define ID_PLAYER 0
#define ID_PLAYER_BULLET 1
#define ID_ENEMY_COMMON 2
#define ID_ENEMY_CONVOY 3
#define ID_ENEMY_SNIPER 4
#define ID_SNIPER_ORB 5

class Ship{
    private:
        u16 timer_to_damage = 5;
    public:
        Axis pos;
        Size size = { 16, 16 };
        Sprite sp;
        u16 hp, id;
        u8 dead = FALSE, damaged = FALSE;
        int dx = 0, dy = 0, spd = 1;

        Ship( int x = 0, int y = 0 , u16 hp = 1 );
        void update();
        void setDamage( s16 dmg );
        u8 shipVsShip( Ship *s );
};

#endif // SHIP_H
