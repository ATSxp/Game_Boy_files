#ifndef MAPS_H
#define MAPS_H

#include <tonc.h>

#define loadMap(name, len, sbb, cbb) \
    memcpy16(&se_mem[ sbb ][ cbb ], name, len); 

void moveBg( u16 bg, int dx, int dy );
void setBg( u16 bg, u16 bgcnt );

#endif // MAPS_H
