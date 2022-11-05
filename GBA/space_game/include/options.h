#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <vector>
#include <string>
#include "void.h"
#include "player.h"
#include "texts_en.h"
#include "texts_ptbr.h"

#include "spr_menu_cursor_img_bin.h"

using namespace std;

extern u16 LANGUAGE;
#define LANGUAGE_EN 0
#define LANGUAGE_PTBR 1

#define translTxt(text)((LANGUAGE)==(LANGUAGE_PTBR) ? (PT_##text) : (EN_##text) )
#define ENvsPTBR( val1, val2 )((LANGUAGE)==(LANGUAGE_PTBR) ? (val2) : (val1))

typedef struct {
    string name;
    void (*action)(void);
    int offset;
    u16 c;
    u32 w, h;
    int x, y;
} ButtonGUI;

void initGUI( u16 slot_obj, ButtonGUI *btn_list, cu16 total_btns, int x, int y, u16 space, vector<u16> clrs, BOOL center);
void updateGUI();
void resetGUI();

#endif // OPTIONS_H
