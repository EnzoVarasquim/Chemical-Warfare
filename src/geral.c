#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "geral.h"

#define MAX_TIROS 30

void desenhar_background(int width, int height) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int x = width/4; x <= width; x += 20) {
        al_draw_line(x, 0, x, height, al_map_rgb(50, 50, 50), 1);
    }
    for (int y = 0; y <= height; y += 20) {
        al_draw_line(width/4, y, width, y, al_map_rgb(50, 50, 50), 1);
    }
}
void desenhar_barra_lateral(int width, int height) {
    al_draw_filled_rectangle(0, 0, width/4, height, al_map_rgb(10, 10, 10));
    al_draw_line(width/4 - 1, 0, width/4 - 1, height, al_map_rgb(50, 50, 50), 1);

    al_draw_line(0, 80, width / 4 - 1, 80, al_map_rgb(50, 50, 50), 1);
    al_draw_line(0, 115, width / 4 - 1, 115, al_map_rgb(50, 50, 50), 1);
    al_draw_line(0, 190, width / 4 - 1, 190, al_map_rgb(50, 50, 50), 1);
    al_draw_line(0, 460, width / 4 - 1, 460, al_map_rgb(50, 50, 50), 1);


}

void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[], int width, int height) {
    *player = init_player(width, height);

    for (int i = 0; i < total_inimigos; i++) {
        int posX, posY;
        float distancia;

        do {
            posX = (width / 4 + 20) + (rand() % (width - width / 4 - 2 * 20 + 1));
            posY = 20 + (rand() % (height - 2 * 20 + 1));


            float diffX = posX - player->x;
            float diffY = posY - player->y;
            distancia = sqrtf(diffX * diffX + diffY * diffY);

        } while (distancia < 150.0f);

        TipoInimigo tipo_aleatorio = (TipoInimigo)(rand() % 3);
        qtd_inimigos[i] = init_inimigo(posX, posY, tipo_aleatorio);
    }

    for (int i = 0; i < MAX_TIROS; i++) {
        tiros[i] = init_tiro();
    }
}
