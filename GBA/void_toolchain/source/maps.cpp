#include "../include/maps.h"

void moveBg( u16 bg, int dx, int dy ){
    vu16 _dx = dx, _dy = dy;
    switch( bg ){
        case 0:
            REG_BG0HOFS = _dx;
            REG_BG0VOFS = _dy;
            break;
        case 1:
            REG_BG1HOFS = _dx;
            REG_BG1VOFS = _dy;
            break;
        case 2:
            REG_BG2HOFS = _dx;
            REG_BG2VOFS = _dy;
            break;
        case 3:
            REG_BG3HOFS = _dx;
            REG_BG3VOFS = _dy;
            break;
    }
}

void setBg( u16 bg, u16 bgcnt ){
    switch( bg ){
        case 0:
            REG_BG0CNT = bgcnt;
            break;
        case 1:
            REG_BG1CNT = bgcnt;
            break;
        case 2:
            REG_BG2CNT = bgcnt;
            break;
        case 3:
            REG_BG3CNT = bgcnt;
            break;

    }
}
