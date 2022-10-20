#ifndef MAPS_H
#define MAPS_H

#include <tonc.h>

#define loadMap( name, len, sbb, cbb ) \
    memcpy16( &se_mem[ sbb ][ cbb ], name, len ); 

#define moveBg( bg, dx, dy ) \
    vs32 _dx##bg, _dy##bg; \
    _dx##bg += dx; \
    _dy##bg += dy; \
    REG_BG##bg##HOFS = _dx##bg; \
    REG_BG##bg##VOFS = _dy##bg; \

#define setBg( bg, bgcnt ) \
    REG_BG##bg##CNT = bgcnt;

#endif // MAPS_H
