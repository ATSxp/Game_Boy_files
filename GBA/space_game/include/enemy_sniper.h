#ifndef ENEMY_SNIPER
#define ENEMY_SNIPER

#include <vector>
#include "void.h"
#include "ship.h"
#include "game.h"

#include "spr_enemy_sniper_img_bin.h"
#include "spr_projectile_for_sniper_img_bin.h"

#define MAX_SNIPERS 1
#define MAX_ORBS 1

using namespace std;

extern vector<Ship>snipers;

void initSniper();
void updateSniper();
void newSniper();

#endif // ENEMY_SNIPER
