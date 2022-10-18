#ifndef LOAD_H
#define LOAD_H

#include <tonc.h>

extern u16 SPRITE_IN_VRAM_OBJ;
extern u16 SPRITE_IN_VRAM_TILE;

#define addSIVO( val ) SPRITE_IN_VRAM_OBJ += val; // facilitando minha vida :)
#define addSIVT( val ) SPRITE_IN_VRAM_TILE += val;

#define loadTileObj( name, size ) \
    if( SPRITE_IN_VRAM_OBJ > 0 ){ \
        memcpy16( &tile_mem[ 4 ][ SPRITE_IN_VRAM_OBJ ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }else { \
        memcpy16( &tile_mem[ 4 ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }

#define loadTile( name, cbb, size ) \
    if( SPRITE_IN_VRAM_TILE > 0 ){ \
        memcpy16( &tile_mem[ cbb ][ SPRITE_IN_VRAM_TILE ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }else { \
        memcpy16( &tile_mem[ cbb ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }

#define loadTileObj8bpp( name, size ) \
    if( SPRITE_IN_VRAM_OBJ > 0 ){ \
        memcpy16( &tile8_mem[ 4 ][ SPRITE_IN_VRAM_OBJ ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }else { \
        memcpy16( &tile8_mem[ 4 ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }

#define loadTile8bpp( name, cbb, size ) \
    if( SPRITE_IN_VRAM_TILE > 0 ){ \
        memcpy16( &tile8_mem[ cbb ][ SPRITE_IN_VRAM_TILE ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }else { \
        memcpy16( &tile8_mem[ cbb ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }

#define loadTileObjLZ77Vram( name, size ) \
    if( SPRITE_IN_VRAM_OBJ > 0 ){ \
        LZ77UnCompVram( name##Tiles, &tile_mem[ 4 ][ SPRITE_IN_VRAM_OBJ ] ); \
        addSIVO( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }else { \
        LZ77UnCompVram( name##Tiles, &tile_mem[ 4 ][ 0 ] ); \
        addSIVO( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }

#define loadTileLZ77Vram( name, cbb, size ) \
    if( SPRITE_IN_VRAM_TILE > 0 ){ \
        LZ77UnCompVram( name##Tiles, &tile_mem[ cbb ][ SPRITE_IN_VRAM_TILE ] ); \
        addSIVT( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }else { \
        LZ77UnCompVram( name##Tiles, &tile_mem[ cbb ][ 0 ] ); \
        addSIVT( size == 8 ? 1 : name##TilesLen / ( size * 2 ) ); \
    }

#define loadPalObj( name, bank ) \
    memcpy16( &pal_obj_bank[ bank ], name##Pal, name##PalLen / 2 );

#define loadPalBg( name, bank ) \
    memcpy16( &pal_bg_bank[ bank ], name##Pal, name##PalLen / 2 );

typedef struct {
    int x; int y;
} Axis;

#endif // LOAD_H
