#include "../include/ship.h"

Ship::Ship( int x, int y, u16 hp ){
    pos.x = x;
    pos.y = y;
    this->hp = hp;
    sp.prio = 1;
}

void Ship::update(){
    if( hp <= 0 ){ dead = TRUE; }

    if( !dead ){
        pos.x += dx;
        pos.y += dy;

        timer_to_damage--;

        if( timer_to_damage <= 0 ){
            timer_to_damage = 5;
            damaged = FALSE;
        }
    }else {
        damaged = FALSE;
    }

    sp.setPos( pos.x, pos.y );
    sp.update();
}

u8 Ship::shipVsShip(Ship *s){
    if( 
        aabb(pos.x, pos.y, size.w, size.h, 
            s->pos.x, s->pos.y, s->size.w, s->size.h) 
      ){
        return TRUE;
    }
    return FALSE;
}

void Ship::setDamage(s16 dmg){
    if( dmg != 0 && !damaged ){
        hp -= dmg;
        damaged = TRUE;
    }
}
