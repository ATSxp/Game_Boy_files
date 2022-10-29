#ifndef ITEMS_H
#define ITEMS_H

#include "void.h"
#include "ship.h"

#define ITEM_MEGA_BULLET 1
#define ITEM_POTION 2
#define ITEM_MULTI_BULLET 3
#define ITEM_BOOST_BULLET 4

class Item {
    public:
        u8 dead = FALSE;
        Axis pos = { 0, 0 };
        Size size = { 8, 8 };
        int dx = 0, dy = 0, spd = 1;
        Sprite sp;
        u16 id;

        Item( u16 slot, int x = 0, int y = 0 );
        u8 itemVsShip( Ship *s );
        void update();
        void chase( int to_x, int to_y );
};

#endif // ITEMS_H
