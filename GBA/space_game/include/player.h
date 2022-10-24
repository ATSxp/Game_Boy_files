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
#define MAX_PLAYER_TIMER_SHOOT 5

extern Ship player;
extern std::vector< Ship > pb;

void initPlayer();
void updatePlayer();
void movePlayer();
void playerShoot();
void newBulletPlayer();
void updateBulletsPlayer();
void animPlayer();
void destroyPlayerBullet(int i);
void gameOverPlayer();

#endif // PLAYER_H
