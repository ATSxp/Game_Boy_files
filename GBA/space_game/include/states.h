#ifndef STATE_H
#define STATE_H

#include "void.h"
#include "game.h"

enum states {
    MENU, GAME
};
extern u16 STATE;

void initStates();
void updateStates();

#endif // STATE_H

