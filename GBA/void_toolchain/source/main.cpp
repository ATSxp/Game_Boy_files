#include <tonc.h>

#include "../include/void.h"

#include "sprites/spr_red_coin.h"
#include "sprites/spr_red_small_coin.h"
#include "sprites/spr_play.h"
#include "sprites/tiles.h"
#include "sprites/spr_block.h"

#include "maps/space.h"

int main(){
    // Gerenciamento de interrupção de hardware
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    irq_enable(II_VBLANK);

    loadPalObj(spr_red_coin, 0);
    loadPalBg(spr_red_coin, 0);
    loadTileObj(spr_red_coin, 16);
    loadTileObj(spr_red_small_coin, 8);
    loadTileObj(spr_play, 16);
    loadTile(tiles, 0, 8);
    loadMap(space_bg0, SPACE_LENGTH, 28, 0);
    loadTileObjLZ77Vram(spr_block, 16);

    setBg(0, BG_SBB(28) | BG_CBB(0) | BG_SIZE0 );
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ_1D | DCNT_OBJ;

    tte_init_se_default(1, BG_CBB(1) | BG_SBB(5));
    tte_init_con();

    initOam();
    
    Sprite coin;
    coin.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16x16 );
    coin.setPos(16, 0);

    Sprite block;
    block.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16);
    block.tid = 13;

    // Update
    while(TRUE){
        VBlankIntrWait(); // Vsync
        key_poll(); // Pesquisa para keystates e chaves repetidas

        coin.update();
        coin.anim(0, 2, 8);

        block.update();
 
        updateOam();
    }
    return 0;
}
