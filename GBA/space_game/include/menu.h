#ifndef MENU_H
#define MENU_H

#include "void.h"
#include "game.h"

using namespace std;

#include "../source/sprites/spr_p_item_slot.h"

extern Scene menu_scene;

void initMenu();
void updateMenu();
void endMenu();

#endif // MENU_H
