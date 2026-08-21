#define MAX_TIROS 30

#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "tiro.h"

Mouse init_mouse(void) {
    Mouse mouse = { 0, 0, false };
    return mouse;
}

Tiro init_tiro(void) {
    Tiro tiro = { 0, 0, 2, 0, 0, false };
    return tiro;
}

void desenhar_tiros(Tiro tiros[]) {
    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            al_draw_filled_rectangle(tiros[i].x - tiros[i].size, tiros[i].y - tiros[i].size, tiros[i].x + tiros[i].size, tiros[i].y + tiros[i].size, al_map_rgb(255, 255, 255));
        }
    }
}

void desenhar_mira(Mouse* mouse) {
    al_draw_circle(mouse->x, mouse->y, 5, al_map_rgb(255, 0, 0), 1);
    al_draw_filled_circle(mouse->x, mouse->y, 4, al_map_rgb(255, 235, 120));
}

void atualizar_posicao_tiros(Tiro tiros[], int largura_tela, int altura_tela) {
    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            tiros[i].x += tiros[i].vel_x;
            tiros[i].y += tiros[i].vel_y;

            //se o tiro sair da tela, desativa ele pra liberar a vaga no array
            if (tiros[i].x < 0 || tiros[i].x > largura_tela || tiros[i].y < 0 || tiros[i].y > altura_tela) {
                tiros[i].ativo = false;
            }
        }
    }
}

void disparar_tiro(Mouse* mouse, Player* player, Tiro tiros[]) {
    if (mouse->left_button == true) {
        //procura por um tiro que não esteja ativo no array
        for (int i = 0; i < MAX_TIROS; i++) {
            if (!tiros[i].ativo) {
                tiros[i].ativo = true;
                tiros[i].x = player->x;
                tiros[i].y = player->y;

                float dx = mouse->x - player->x;
                float dy = mouse->y - player->y;
                float distancia = sqrtf(dx * dx + dy * dy);
                float velocidade_total = 8.0f;

                if (distancia != 0) {
                    tiros[i].vel_x = (dx / distancia) * velocidade_total;
                    tiros[i].vel_y = (dy / distancia) * velocidade_total;
                }

                break;
            }
        }
    }
}