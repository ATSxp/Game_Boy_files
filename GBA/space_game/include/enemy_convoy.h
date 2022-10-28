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
#define MAX_CONVOYS_TIMER_SPAWN 1000

void initConvoys();
void updateConvoys();
void newConvoy();
void updateItems();
void dropMegaBullet( int x, int y );
void itemIdValue( u16 id );

#endif // ENEMY_CONVOY_H
