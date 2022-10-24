#ifndef GAME_H
#define GAME_H

#include "void.h"
#include "player.h"
#include "enemy_common.h"

/* #include "../source/maps/space_bg0.h" */
/* #include "../source/maps/space_bg1.h" */
#include "../source/maps/space.h"
#include "../source/sprites/tileset_space.h"
#include "../source/sprites/spr_hp_player.h"

void initGame();
void initHudPlayer();
void updateGame();
void updateHudPlayer();

#endif // GAME_H
