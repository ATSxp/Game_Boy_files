#include "../include/items.h"

Item::Item( u16 slot, int x, int y ){
    this->sp.newSprite(slot);
    this->pos.x = x;
    this->pos.y = y;
}

void Item::update(){
    if( !dead ){

        pos.x += dx;
        pos.y += dy;
        sp.update();
        sp.setPos(pos.x, pos.y);
    }
}

u8 Item::itemVsShip( Ship *s ){
    if( aabb(pos.x + size.w / 2, pos.y + size.h / 2, size.w, size.h, 
            s->pos.x, s->pos.y, s->size.w, s->size.h) 
    ){
        return TRUE;
    }
    return FALSE;
}

void Item::chase(int to_x, int to_y){
    u16 _range = 320;

    s16 ddx = to_x - pos.x, ddy = to_y - pos.y;
    s16 dst = Sqrt( ( ddx * ddx ) + ( ddy * ddy ) );

    if( dst <= _range ){
        if( ABS( ddx ) >= spd ){
            dx = ddx > 0 ? spd : -spd;
        }

        if( ABS( ddy ) >= spd ){
            dy = ddy > 0 ? spd : -spd;
        }
    }
}
