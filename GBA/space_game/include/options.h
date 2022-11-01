#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include "void.h"
#include "player.h"
#include "texts_en.h"
#include "texts_ptbr.h"

#include "../source/sprites/spr_p_item_slot.h"

using namespace std;

extern u16 LANGUAGE;
#define LANGUAGE_EN 0
#define LANGUAGE_PTBR 1

#define translTxt(text)((LANGUAGE)==(LANGUAGE_PTBR) ? (PT_##text) : (EN_##text) )

typedef struct {
    string name;
    void (*action)(void);
    u16 c;
    u32 w, h;
    int x, y;
} ButtonGUI;

void initGUI( int x, int y, u16 space, BOOL center );
void updateGUI( ButtonGUI btns[], u16 total_btns );

#endif // OPTIONS_H
