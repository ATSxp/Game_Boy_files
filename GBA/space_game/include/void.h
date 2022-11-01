#ifndef VOID_H
#define VOID_H

#include <tonc.h>
#include <string>
#include <stdio.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define MAX_SPRITES_IN_OAM 128

#define pmem( index, value ) \
    if( value ) \
        sram_mem[ index ] = value; \
    else \
        return sram_mem[ index ];

#define moveBg( bg, dx, dy ) \
    vs32 _dx##bg, _dy##bg; \
    _dx##bg += dx; \
    _dy##bg += dy; \
    REG_BG##bg##HOFS = _dx##bg; \
    REG_BG##bg##VOFS = _dy##bg; \

#define addSIVO( val ) SPRITE_IN_VRAM_OBJ += val; // facilitando minha vida :)
#define addSIVT( val ) SPRITE_IN_VRAM_TILE += val;
#define addPIBB( val ) PAL_IN_BANK_BG += val;
#define addPIBO( val ) PAL_IN_BANK_OBJ += val;

#define loadBmp(name) memcpy(pal_bg_mem, name##Pal, name##PalLen); memcpy(vid_mem, name##Bitmap, name##BitmapLen);
#define loadBmp16(name) memcpy16(vid_mem, name##Bitmap, name##BitmapLen/2);

#define loadTileObj( name, size ) \
    if( SPRITE_IN_VRAM_OBJ > 0 ){ \
        memcpy16( &tile_mem[ 4 ][ SPRITE_IN_VRAM_OBJ ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? name##TilesLen / ( size * 4 ) : ( size == 16 ? name##TilesLen / ( size * 2 ) : ( name##TilesLen / size ) + 2 ) ); \
    }else { \
        memcpy16( &tile_mem[ 4 ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? name##TilesLen / ( size * 4 ) : ( size == 16 ? name##TilesLen / ( size * 2 ) : ( name##TilesLen / size ) + 2 ) ); \
    }

#define loadTile( name ) \
    if( SPRITE_IN_VRAM_TILE > 0 ){ \
        memcpy16( &tile_mem[ 0 ][ SPRITE_IN_VRAM_TILE ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( name##TilesLen / 8 ); \
    }else { \
        memcpy16( &tile_mem[ 0 ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( name##TilesLen / 8 ); \
    }

#define loadTileObj8bpp( name, size ) \
    if( SPRITE_IN_VRAM_OBJ > 0 ){ \
        memcpy16( &tile8_mem[ 4 ][ SPRITE_IN_VRAM_OBJ ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? name##TilesLen / ( size * 4 ) : name##TilesLen / ( size * 2 ) ); \
    }else { \
        memcpy16( &tile8_mem[ 4 ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVO( size == 8 ? name##TilesLen / ( size * 4 ) : name##TilesLen / ( size * 2 ) ); \
    }

#define loadTile8bpp( name ) \
    if( SPRITE_IN_VRAM_TILE > 0 ){ \
        memcpy16( &tile8_mem[ 0 ][ SPRITE_IN_VRAM_TILE ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( name##TilesLen / 8 ); \
    }else { \
        memcpy16( &tile8_mem[ 0 ][ 0 ], name##Tiles, name##TilesLen / 2 ); \
        addSIVT( 1 ); \
    }

#define loadTileObjLZ77Vram( name, size ) \
    if( SPRITE_IN_VRAM_OBJ > 0 ){ \
        LZ77UnCompVram( name##Tiles, &tile_mem[ 4 ][ SPRITE_IN_VRAM_OBJ ] ); \
        addSIVO( size == 8 ? name##TilesLen / ( size * 4 ) : name##TilesLen / ( size * 2 ) + 1 ); \
    }else { \
        LZ77UnCompVram( name##Tiles, &tile_mem[ 4 ][ 0 ] ); \
        addSIVO( size == 8 ? name##TilesLen / ( size * 4 ) : name##TilesLen / ( size * 2 ) + 1 ); \
    }

#define loadTileLZ77Vram( name ) \
    if( SPRITE_IN_VRAM_TILE > 0 ){ \
        LZ77UnCompVram( name##Tiles, &tile_mem[ 0 ][ SPRITE_IN_VRAM_TILE ] ); \
        addSIVT( name##TilesLen / 8 ); \
    }else { \
        LZ77UnCompVram( name##Tiles, &tile_mem[ 0 ][ 0 ] ); \
        addSIVT( name##TilesLen / 8 ); \
    }

#define loadPalObj( name ) \
    if( PAL_IN_BANK_OBJ > 0 ){ \
        memcpy16( &pal_obj_mem[ PAL_IN_BANK_OBJ ], name##Pal, name##PalLen / 2 ); \
        addPIBO( ( name##PalLen / 2 ) + 1 ); \
    }else { \
        memcpy16( &pal_obj_mem[ 0 ], name##Pal, name##PalLen / 2 ); \
        addPIBO( ( name##PalLen / 2 ) + 1 ); \
    } \

#define loadPalBg( name ) \
    if( PAL_IN_BANK_BG > 0 ){ \
        memcpy16( &pal_bg_mem[ PAL_IN_BANK_BG ], name##Pal, name##PalLen / 2 ); \
        addPIBB( ( name##PalLen / 2 ) + 1 ); \
    }else { \
        memcpy16( &pal_bg_mem[ 0 ], name##Pal, name##PalLen / 2 ); \
        addPIBB( ( name##PalLen / 2 ) + 1 ); \
    } \

typedef struct {
    int x; int y;
} Axis;

typedef struct {
    u32 w; u32 h;
} Size;

typedef struct {
    int x, xmin, xmax, xpage;
    int y, ymin, ymax, ypage;
} VIEWPORT;

typedef struct {
    void (*init)(void);
    void (*end)(void);
    void (*update)(void);
} Scene;

extern u16 SPRITE_TOTAL_OAM, ID_SLOT_SCAN;
extern OBJ_ATTR OBJ_BUFFER[ MAX_SPRITES_IN_OAM ];
extern u16 mget(int x, int y, cu16 *bg, u16 map_width);
extern u8 mset( int x, int y, u8 t );
extern u16 SPRITE_IN_VRAM_OBJ;
extern u16 SPRITE_IN_VRAM_TILE;
extern u16 PAL_IN_BANK_OBJ;
extern u16 PAL_IN_BANK_BG;

// Class Sprite /////////////////////////////////////////
class Sprite {
    private:
    public:
        OBJ_ATTR *spr;
        u16 slot_in_oam;
        Axis pos = { 0, 0 };
        Size size;
        u16 tid = 0, pal = 0, prio = 0, tick = 0;
        u8 flip_h = FALSE, flip_v = FALSE;

        Sprite( s16 slot = -1 );
        void newSprite( u16 slot = 0 );
        void setAttr( u16 attr0, u16 attr1 );
        void setPos( int nx = 0, int ny = 0 );
        void setTileId( u16 id );
        Axis getPos();
        Size getSize();
        int getWidth();
        int getHeight();
        void update();
        void hide();
        void unhide();
        void anim(u16 start, u16 frames, u16 rate = 8);
};

u8 aabb( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 );
u8 spriteIsHided( u16 slot );
void initOam();
void updateOam();
u8 checkSpriteIsHided( u16 id );

// Class Map ////////////////////////////////////////////
class Map{
    private:
        u16 flags, cnt;
        void colcpy( int tx, int ty );
        void rowcpy( int tx, int ty );

    public:
        SCR_ENTRY *src, *dst;
        u32 w, h;
        FIXED x, y;

        VIEWPORT vp;

        Map( int bgnr, u32 ctrl, const void *map, u32 map_w, u32 map_h );
        void update();
        void setPos(int x, int y);
        u16 getTile( int tx, int ty );
        void setTile( int tx, int ty, u8 t );
};

// Class Fade ////////////////////////////////////////////
/* class Fade{ */
/*     private: */
/*         const COLOR *src; */
/*         COLOR clr; */
/*         u32 num_clrs; */
/*     public: */
/*         u16 value; */
/*         int vel; */

/*         Fade( const COLOR *src, COLOR clr = CLR_BLACK, u32 num_clrs = 1 ); */
/*         void update(); */
/*         void fadeIn( u16 spd = 1 ); */
/*         void fadeOut( u16 spd = 1 ); */
/* }; */

extern BOOL shaking;

void tte_write_str( std::string text );
void nocash_puts_str( std::string text );
void setScene(Scene next);
void shakeScreen( u32 amt, u32 bg, u32 mag );
void updateShakeScreen();
void updateScene();
void initVoid();
void updateVoid();
void resetVoid();

#endif // VOID_H
