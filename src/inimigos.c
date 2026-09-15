#include <allegro5/allegro_primitives.h>
#include "inimigos.h"

#define MAX_TIROS 30

Inimigo init_inimigo(int x, int y) {
	Inimigo inimigo = { x, y, 20, 2, 5, true };
	return inimigo;
}

void desenhar_inimigo(Inimigo inimigo) {
    if (inimigo.vivo == true) {
        al_draw_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 120, 0), 2);
        al_draw_filled_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 0, 0));
    }
   
}
// segue player
void seguir_player(Inimigo* inimigo, int player_x, int player_y)
{
    if (!inimigo->vivo) return;

    float velocidade = inimigo->vel * 0.5f;

    if (inimigo->x < player_x)
        inimigo->x += velocidade;
    else if (inimigo->x > player_x)
        inimigo->x -= velocidade;

    if (inimigo->y < player_y)
        inimigo->y += velocidade;
    else if (inimigo->y > player_y)
        inimigo->y -= velocidade;
}
// colisão entre inimigos
void colisao_inimigos(Inimigo* inimigo1, Inimigo* inimigo2)
{
    if (!inimigo1->vivo || !inimigo2->vivo)
        return;

    if (inimigo1->x < inimigo2->x + inimigo2->size &&
        inimigo1->x + inimigo1->size > inimigo2->x &&
        inimigo1->y < inimigo2->y + inimigo2->size &&
        inimigo1->y + inimigo1->size > inimigo2->y)
    {
        inimigo1->x -= 1;
        inimigo1->y -= 1;

        inimigo2->x += 1;
        inimigo2->y += 1;
    }
}
void colisao_tiro(Inimigo* inimigo, Tiro tiros[]) {
    if (inimigo->vivo == false) return;

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
                break;
            }
                
        }
    }
}
