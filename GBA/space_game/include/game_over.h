#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "void.h"
#include "game.h"
#include "menu.h"

#define MAX_GAME_OVER_TIMER 90

extern u16 gmo_timer;

void initGameOver();
void updateGameOver();
void resetGame();

void _tryAgain();
void _backToMenu();

#endif // GAME_OVER_H
