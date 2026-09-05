#include <allegro5/allegro_primitives.h>
#include "inimigos.h"

#define MAX_TIROS 30

Inimigo init_inimigo(void) {
	Inimigo inimigo = { 340, 240, 20, 2, 5, true };
	return inimigo;
}

void desenhar_inimigo(Inimigo inimigo) {
    if (inimigo.vivo == true) {
        al_draw_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 120, 0), 2);
        al_draw_filled_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 0, 0));
    }
   
}

void colisao_tiro(Inimigo* inimigo, Tiro tiros[]) {
    if (inimigo->vivo == true) {
        for (int i = 0; i < MAX_TIROS; i++) {
            if (tiros[i].ativo) {

                if (tiros[i].x + tiros[i].size >= inimigo->x &&       //esquerda
                    tiros[i].x <= inimigo->x + inimigo->size &&       //direita
                    tiros[i].y + tiros[i].size >= inimigo->y &&       //cima
                    tiros[i].y <= inimigo->y + inimigo->size)         //baixo
                {
                    tiros[i].ativo = false;
                    inimigo->vida--;
                }

                //matando o inimigo
                if (inimigo->vida == 0) {
                    inimigo->vivo = false;
                }

            }
        }
    }
}
