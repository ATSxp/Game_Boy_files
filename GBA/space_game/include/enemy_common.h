#ifndef ENEMY_COMMON_H
#define ENEMY_COMMON_H

#include <vector>
#include "void.h"
#include "ship.h"
#include "player.h"

#include "../source/sprites/spr_enemy_common.h"

#define MAX_ENEMIES 16
#define MAX_ENEMY_TIMER_SPAWN 15

extern std::vector< Ship > enemies;

void initEnemy();
void newEnemy();
void updateEnemies();
void removeEnemiesAfterPassLimit( size_t i );
void removeEnemies( size_t i );
void animEnemyExplode(int i);
void destroyEnemy(int i);

#endif // ENEMY_COMMON_H
