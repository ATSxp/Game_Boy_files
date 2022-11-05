#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "void.h"
#include "ship.h"
#include "enemy_common.h"
#include "enemy_sniper.h"

#include "spr_player_pal_bin.h"
#include "spr_player_img_bin.h"
#include "spr_bullets_img_bin.h"
#include "spr_bullet_explosion_img_bin.h"

#define MAX_HP_PLAYER 8
#define MAX_PLAYER_BULLETS 16
#define MAX_PLAYER_IMORTALITY 700

extern Ship player;
extern std::vector< Ship > pb;
extern s16 p_points, p_mega_bullets, p_potions, p_boost_bullets, p_multi_bullets, p_imortal_item;
extern BOOL p_imortal, p_can_move;
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
void useImortalityItem();

#endif // PLAYER_H
