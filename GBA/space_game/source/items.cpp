#include "../include/items.h"

Item::Item( u16 slot, int x, int y ){
    sp.newSprite(slot);
    pos.x = x;
    pos.y = y;
}

void Item::update(){
    if( !dead ){
        pos.x += dx;
        pos.y += dy;
    }
    sp.setPos(pos.x, pos.y);
    sp.update();
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
    s16 a = ArcTan2(ddx, ddy);

    if( dst <= _range ){
        if( ABS( ddx ) >= spd ){
            dx = ( ( lu_cos(a) >> 8 ) - ( spd << 4 ) ) >> 2;
        }

        if( ABS( ddy ) >= spd ){
            dy = ( ( lu_sin(a) >> 8 ) - ( spd << 4 ) ) >> 2;
        }
    }
}
