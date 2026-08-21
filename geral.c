#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "geral.h"

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

void reset(Player* player, Inimigo* inimigo, Tiro tiros[]) {
    *player = init_player();
    *inimigo = init_inimigo();

    for (int i = 0; i < MAX_TIROS; i++) {
        tiros[i] = init_tiro();
    }
}