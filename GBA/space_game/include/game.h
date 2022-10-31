#ifndef GAME_H
#define GAME_H

#include <string>
#include "void.h"
#include "player.h"
#include "enemy_common.h"
#include "enemy_convoy.h"
#include "options.h"
#include "game_over.h"

#include "../source/maps/space.h"
#include "../source/sprites/tileset_space.h"
#include "../source/sprites/spr_hp_player.h"
#include "../source/sprites/spr_mega_bullet_slot_and_item.h"
#include "../source/sprites/spr_life_item.h"
#include "../source/sprites/spr_boost_bullet_item.h"
#include "../source/sprites/spr_mega_bullet.h"
#include "../source/sprites/spr_multi_bullets_item.h"
#include "../source/sprites/spr_imortality_item.h"
#include "../source/sprites/spr_buttons.h"
#include "../source/sprites/spr_p_imortality_effect.h"

extern Scene game_scene;

void initGame();
void initHudPlayer();
void updateGame();
void endGame();
void updateHudPlayer();

void destroyPlayerBullet(int i);
void removeEnemies( Ship *t );
void animEnemyExplode( Ship *t );
void destroyEnemy( Ship *t, std::vector<Ship> *v, size_t i );

#endif // GAME_H
