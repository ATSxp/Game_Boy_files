#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "void.h"
#include "player.h"

#define MAX_GAME_OVER_TIMER 90

/* extern u8 r_main; */
extern u16 gmo_timer;

void updateGameOver();

#endif // GAME_OVER_H
