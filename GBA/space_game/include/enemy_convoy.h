#ifndef ENEMY_CONVOY_H
#define ENEMY_CONVOY_H

#include <vector>
#include "void.h"
#include "ship.h"
#include "player.h"
#include "game.h"
#include "items.h"

#include "../source/sprites/spr_enemy_convoy.h"

#define MAX_CONVOYS 5
#define MAX_ITEMS 2
#define MAX_CONVOYS_TIMER_SPAWN 700

extern std::vector<Item> items;
extern u16 check_slot_to_item;

void initConvoys();
void updateConvoys();
void newConvoy();
void updateItems();
void dropItem( int x, int y );
void itemIdValue( u16 id );

INLINE void dropItemType( int x, int y, u16 tid, u16 id ){
    if( spriteIsHided( 53 + check_slot_to_item ) ){
        Item i( 53 + check_slot_to_item, x, y );
        i.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
        i.sp.tid = tid;
        i.sp.prio = 1;
        i.id = id;
        i.spd = ( 1 >> 6 ) >> 2;

        items.push_back( Item( i ) );
    }
}

#endif // ENEMY_CONVOY_H
