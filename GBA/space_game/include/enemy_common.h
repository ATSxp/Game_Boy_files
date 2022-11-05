#ifndef ENEMY_COMMON_H
#define ENEMY_COMMON_H

#include <vector>
#include "void.h"
#include "ship.h"
#include "player.h"
#include "game.h"

#include "spr_enemy_common_pal_bin.h"
#include "spr_enemy_common_img_bin.h"

#define MAX_ENEMIES 16
#define MAX_ENEMY_TIMER_SPAWN 50

extern std::vector< Ship > enemies;

void initEnemy();
void newEnemy();
void updateEnemies();

#endif // ENEMY_COMMON_H
