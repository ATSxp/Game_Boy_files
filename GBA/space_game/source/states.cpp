#include "../include/states.h"

u16 STATE = GAME;

void initStates(){
    switch( STATE ){
        case MENU:
            break;
        case GAME:
            initGame();
            break;
    }
}

void updateStates(){
    switch( STATE ){
        case MENU:
            break;
        case GAME:
            updateGame();
            break;
    }
}
