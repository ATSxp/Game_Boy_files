/*! TODO: 
 * - Tela de Game over;
 * - Boss;
 * - Sfx;
 * - Bg music;
 * - Cutscenes ( Bitmap );
*/

#include "../include/void.h"
#include "../include/menu.h"
#include "../include/demo_menu.h"
#include "../include/game.h"

u32 world_seconds = -1;

int main(){
    // Gerenciamento de interrupção de hardware
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    REG_TM2D = -0x4000;
    REG_TM2CNT = TM_ENABLE | TM_FREQ_1024;
    REG_TM3CNT = TM_ENABLE | TM_CASCADE;

    setScene(demo_menu_scene);
    /* setScene(menu_scene); */
    /* setScene(game_scene); */

    while(TRUE){
        VBlankIntrWait(); // Vsync
        key_poll(); // Pesquisa para keystates e chaves repetidas
        updateScene();
        if( REG_TM3D != world_seconds ){ world_seconds = REG_TM3D; }
    }
    return 0;
}
