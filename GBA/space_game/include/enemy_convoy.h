#ifndef ENEMY_CONVOY_H
#define ENEMY_CONVOY_H

#include <vector>
#include "void.h"
#include "ship.h"
#include "player.h"
#include "game.h"
#include "items.h"

#include "spr_enemy_convoy_pal_bin.h"
#include "spr_enemy_convoy_img_bin.h"

#define MAX_CONVOYS 5
#define MAX_ITEMS 2
#define MAX_CONVOYS_TIMER_SPAWN 700

typedef struct {
    u16 tid, id;
} Item_list;

extern std::vector<Item> items;
extern u16 check_slot_to_item;

void initConvoys();
void updateConvoys();
void newConvoy();
void updateItems();
void itemIdValue( u16 id );

#endif // ENEMY_CONVOY_H
