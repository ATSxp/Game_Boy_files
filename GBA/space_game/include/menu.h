#ifndef MENU_H
#define MENU_H

#include "void.h"
#include "game.h"
#include "texts_ptbr.h"
#include "texts_en.h"

using namespace std;

#include "spr_menu_cursor_pal_bin.h"

extern Scene menu_scene;

void initMenu();
void updateMenu();
void endMenu();

#endif // MENU_H
