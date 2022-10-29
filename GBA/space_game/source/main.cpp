/*! TODO: 
 * - Adicionar seed random;
 * - Tela de Game over;
 * - Animação de morte do Jogador;
 * - Boss;
 * - Sfx;
 * - Bg music;
 * - inimigo variado ( o cargueiro );
 * - HUD;
 * - itens ( moedas, mega-balas, imortalidade, tiros-super-rápidos );
 */

#include "../include/void.h"
#include "../include/states.h"

/* u8 r_main = FALSE; */

int main(){
    // Gerenciamento de interrupção de hardware
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    reset_main:

    initStates();

    while(TRUE){
        VBlankIntrWait(); // Vsync
        key_poll(); // Pesquisa para keystates e chaves repetidas

        updateStates();

        /* if( r_main ){ r_main = FALSE; goto reset_main; } */
    }
    return 0;
}
