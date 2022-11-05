#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "void.h"
#include "player.h"
#include "enemy_common.h"
#include "enemy_convoy.h"
#include "enemy_sniper.h"
#include "options.h"
#include "game_over.h"

#include "../source/maps/space.h"
#include "tileset_space_pal_bin.h"
#include "tileset_space_img_bin.h"
#include "spr_hp_player_img_bin.h"
#include "spr_life_item_img_bin.h"
#include "spr_boost_bullet_item_img_bin.h"
#include "spr_mega_bullet_slot_and_item_img_bin.h"
#include "spr_mega_bullet_img_bin.h"
#include "spr_multi_bullets_item_img_bin.h"
#include "spr_imortality_item_img_bin.h"
#include "spr_buttons_img_bin.h"
#include "spr_p_imortality_effect_img_bin.h"

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
void checkPlayerDamage( Ship *t );

#endif // GAME_H
