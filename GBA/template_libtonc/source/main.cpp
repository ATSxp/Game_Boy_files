#include <tonc.h> // Libtonc

int main(){
    // Gerenciamento de interrupção de hardware
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);

    // Mode de vídeo 0 e Fundo 0 ativados
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
    
    while(TRUE){
        // Update
        key_poll(); // Pesquisa para keystates e chaves repetidas
        
        // Draw
        VBlankIntrWait(); // Vsync

    }
    return 0;
}
