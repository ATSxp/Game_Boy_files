#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "void.h"
#include "ship.h"
#include "enemy_common.h"

#include "../source/sprites/spr_player.h"
#include "../source/sprites/spr_bullets.h"
#include "../source/sprites/spr_bullet_explosion.h"

#define MAX_HP_PLAYER 8
#define MAX_PLAYER_BULLETS 16

extern Ship player;
extern std::vector< Ship > pb;
extern s16 p_points, p_mega_bullets, p_potions, p_boost_bullets, p_multi_bullets;
extern u16 MAX_PLAYER_TIMER_SHOOT;

void initPlayer();
void updatePlayer();
void movePlayer();
void playerShoot();
void newBulletPlayer();
void updateBulletsPlayer();
void animPlayer();
void gameOverPlayer();
void lifePlayer();
void newMegaBulletPlayer();
void deadPlayer();

#endif // PLAYER_H
