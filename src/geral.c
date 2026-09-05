#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "geral.h"
#include "inimigos.h"

#define MAX_TIROS 30

//tela
int width = 640;
int height = 480;


void desenhar_background(void) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int x = 0; x <= width; x += 20) {
        al_draw_line(x, 0, x, height, al_map_rgb(50, 50, 50), 1);
    }
    for (int y = 0; y <= height; y += 20) {
        al_draw_line(0, y, width, y, al_map_rgb(50, 50, 50), 1);
    }
}

void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[]) {
    *player = init_player();

    for (int i = 0; i < total_inimigos; i++) {
        int posX = qtd_inimigos[i].size + (rand() % (width - 2 * qtd_inimigos[i].size + 1));
        int posY = qtd_inimigos[i].size + (rand() % (height - 2 * qtd_inimigos[i].size + 1));
        qtd_inimigos[i] = init_inimigo(posX, posY);
    }

    for (int i = 0; i < MAX_TIROS; i++) {
        tiros[i] = init_tiro();
    }
}
