#ifndef VOID_H
#define VOID_H

#include "tonc.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int TICK;
extern int NUM_OBJS; // Total atual de Objetos na OAM
extern uint LIMIT_OBJS; // Quantidade limite de Objetos na OAM
extern OBJ_ATTR obj_buffer[128]; // Buffer de Objetos, até 128

#define SQUARE 0 // Shape do sprite

/* typedef struct Tiles{ cu16 *data; int len; } Tiles; // Tiles e seus dados e comprimento do array */
/* typedef struct Palette{ cu16 *data; int len; } Palette; // Paleta e seus dados e comprimento do array */
/* typedef struct Map{ cu16 *data; vu16 len; int w; } Map; */

typedef struct Entity{
    int x; int y;
    int w; int h;
    int dx; int dy; 
    OBJ_ATTR *sprite;
} Entity;

void initVoid(); // Inicia a "Engine", chamando o Vsync e o Registro dos botões
void initOam(); // inicia a OAM definindo o limite de Objetos na OAM (default: LIMIT_OBJS = 128)
void initEnt(Entity *o, int x, int y); // Inicia uma Entidade

// Carrega um Sprite na VRAM ( Só nos Modos de Tiles )
#define loadSprite(tile, index) memcpy(pal_obj_mem, tile##Pal, tile##PalLen); memcpy(&tile_mem[4][index], tile##Tiles, tile##TilesLen);
#define loadSprite8(tile, index) memcpy16(pal_obj_mem, tile##Pal, tile##PalLen/2); memcpy16(&tile8_mem[4][index], tile##Tiles, tile##TilesLen/2);

// Carrega imagens Bitmap na memória ( Só nos Modos de Bitmap )
#define loadBmp(name) memcpy(vid_mem, name##Pal, name##PalLen); memcpy(vid_mem, name##Bitmap, name##BitmapLen);
#define loadBmp16(name) memcpy16(vid_mem, name##Bitmap, name##BitmapLen/2);

// Carrega um Tileset na VRAM
#define loadTileset(name, cbb, index) memcpy(pal_bg_mem, name##Pal, name##PalLen); memcpy(&tile_mem[cbb][index], name##Tiles, name##TilesLen);
#define loadTileset8(name, cbb, index) memcpy16(pal_bg_mem, name##Pal, name##PalLen/2); memcpy16(&tile8_mem[cbb][index], name##Tiles, name##TilesLen/2);

// Carrega um Mapa
#define loadMap(name, len, sbb, cbb) memcpy16(&se_mem[sbb][cbb], name, len); 
void loadObjectsToOam(); // Carrega os Objetos na OAM

OBJ_ATTR getSprite(uint id);
u16 getSpriteAttr(Entity *o, u16 attr); // Retorna um dos Atributos do sprite
void setMode(uint mode);
void setObjectPosition(OBJ_ATTR *o, int x, int y); // Posiciona o Objeto
void setAnim(OBJ_ATTR *o, uint start, uint frames, uint delay); // Animação para o Objeto

int PixelToTileX(int x);
int PixelToTileY(int y);
int TileToPixelX(int x);
int TileToPixelY(int y);

void flipSprite(OBJ_ATTR *o, bool f);
void flipEnt(Entity *o, bool f);
void moveBg(uint bg, int x, int y);
OBJ_ATTR *spr(uint id, int x, int y, bool f, uint shape, uint bpp, uint size); // Desenha um Sprite
void sprEnt(Entity *o, uint id, bool f, uint shape, uint bpp, uint size);

void updateEnt(Entity *o);
/* cu16 mget(vu32 x, vu32 y, Map map); */

#ifdef __cplusplus
};
#endif

#endif // VOID_H
