#include <tonc.h>

#include "../include/void.h"

#include "sprites/spr_red_coin.h"
#include "sprites/spr_red_small_coin.h"
#include "sprites/spr_play.h"
#include "sprites/tiles.h"
#include "sprites/spr_block.h"

#include "maps/space.h"
#include "maps/space_2.h"
#include "maps/space_solids.h"

u16 sol(int x, int y){
    return mget( x / 8, y / 8, space_solidsMap, SPACE_WIDTH) == 0x1;
}

u8 isSolid(int x, int y){
    if(
        sol( x, y ) || sol( x + 15, y ) ||
        sol( x, y + 15 ) || sol( x + 15, y + 15 )
      ){
        return TRUE;
    }
    
    return FALSE;
}

int main(){
    // Gerenciamento de interrupção de hardware
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    irq_enable(II_VBLANK);

    loadPalObj(spr_red_coin);
    loadPalObj(spr_red_coin);
    loadPalBg(space_2);
    loadTileObj(spr_red_coin, 16);
    loadTileObj(spr_red_small_coin, 8);
    loadTileObj(spr_play, 16);
    loadTileObjLZ77Vram(spr_block, 16);

    loadTileLZ77Vram(space_2);
    Map space(0, BG_CBB(0) | BG_SBB(29) | BG_PRIO(1), space_2Map, SPACE_WIDTH, SPACE_HEIGHT);
    space.vp = {
        0, 0, 512, 240,
        0, 0, 512, 160,
    };

    /* setBg( 0, BG_SBB(28) | BG_CBB(0) | BG_PRIO(1) ); */
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ_1D | DCNT_OBJ;

    tte_init_se_default(1, BG_CBB(2) | BG_SBB(28) | BG_PRIO(0));
    tte_init_con();

    initOam();

    Sprite coin;
    coin.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16x16 );
    coin.setPos(16, 0);

    Sprite block;
    block.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16);
    block.tid = 14;

    int px = SCREEN_WIDTH / 2, py = SCREEN_HEIGHT / 2, dx = 0, dy = 0;

    // Update
    while(TRUE){
        VBlankIntrWait(); // Vsync
        key_poll(); // Pesquisa para keystates e chaves repetidas

        space.update();

        coin.update();
        coin.anim(0, 2, 16);

        block.update();
        block.anim(3, 2, 16);

        int camx = px - 120, camy = py - 60;
        space.setPos(camx, camy) ;

        block.setPos(px - space.x, py - space.y);

        dx = key_tri_horz();
        dy = key_tri_vert();

        if( isSolid(px, py) ){
            dx = 0;
            dy = 0;
        }

        if( key_hit( KEY_A ) ){
            space.setTile(0, 0, 0x0);
        }

        px += dx;
        py += dy;
 
        updateOam();
    }
    return 0;
}
