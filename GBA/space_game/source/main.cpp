/*! TODO: 
 * - Adicionar seed random;
 * - Tela de Game over;
 * - Animação de morte do Jogador;
 * - Boss;
 * - Sfx;
 * - Bg music;
 * - inimigo variado ( o cargueiro );
 * - HUD;
 */

#include "../include/void.h"
#include "../include/states.h"

int main(){
    // Gerenciamento de interrupção de hardware
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    initStates();

    while(TRUE){
        VBlankIntrWait(); // Vsync
        key_poll(); // Pesquisa para keystates e chaves repetidas

        updateStates();
    }
    return 0;
}
