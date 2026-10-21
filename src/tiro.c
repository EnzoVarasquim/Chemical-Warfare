
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "tiro.h"

Mouse init_mouse(void) {
    Mouse mouse = { 0, 0, false };
    return mouse;
}

Tiro init_tiro(void) {
    Tiro tiro = { 0 };

    tiro.size = 2;
    tiro.ativo = false;
    tiro.formato = FORMATO_CIRCULO;

    // Tiro padrão branco
    tiro.cor_r = 255;
    tiro.cor_g = 255;
    tiro.cor_b = 255;

    tiro.vel_x = 0;
    tiro.vel_y = 0;

    return tiro;
}

void desenhar_tiros(Tiro tiros[]) {
    for (int i = 0; i < MAX_TIROS; i++) {

        // Ignora tiros inativos
        if (!tiros[i].ativo) {
            continue;
        }

        Tiro* t = &tiros[i];

        ALLEGRO_COLOR cor = al_map_rgb(
            t->cor_r,
            t->cor_g,
            t->cor_b
        );

        switch (t->formato) {

        case FORMATO_CIRCULO:
            al_draw_filled_circle(
                t->x,
                t->y,
                t->size,
                cor
            );
            break;

        case FORMATO_QUADRADO:
            al_draw_filled_rectangle(
                t->x - t->size,
                t->y - t->size,
                t->x + t->size,
                t->y + t->size,
                cor
            );
            break;

        case FORMATO_TRIANGULO:
            al_draw_filled_triangle(
                t->x,
                t->y - t->size * 1.5f,
                t->x - t->size,
                t->y + t->size,
                t->x + t->size,
                t->y + t->size,
                cor
            );
            break;

        default:
            // Formato desconhecido: círculo branco
            al_draw_filled_circle(
                t->x,
                t->y,
                t->size,
                al_map_rgb(255, 255, 255)
            );
            break;
        }
    }
}

void desenhar_mira(Mouse* mouse) {
    al_draw_circle(
        mouse->x,
        mouse->y,
        5,
        al_map_rgb(255, 0, 0),
        1
    );

    al_draw_filled_circle(
        mouse->x,
        mouse->y,
        4,
        al_map_rgb(255, 235, 120)
    );
}

void atualizar_posicao_tiros(
    Tiro tiros[],
    int largura_tela,
    int altura_tela
) {
    for (int i = 0; i < MAX_TIROS; i++) {

        if (!tiros[i].ativo) {
            continue;
        }

        tiros[i].x += tiros[i].vel_x;
        tiros[i].y += tiros[i].vel_y;

        // Desativa o tiro quando sai da tela
        if (tiros[i].x < 0 ||
            tiros[i].x > largura_tela ||
            tiros[i].y < 0 ||
            tiros[i].y > altura_tela) {

            tiros[i].ativo = false;
        }
    }
}

void disparar_tiro(
    Mouse* mouse,
    Player* player,
    Tiro tiros[],
    TipoArma arma,
    SistemaQuimico* sq
) {
    if (!player->vivo || !mouse->left_button || sq == NULL) {
        return;
    }

    for (int i = 0; i < MAX_TIROS; i++) {

        // Procura uma vaga livre
        if (tiros[i].ativo) {
            continue;
        }

        Tiro* t = &tiros[i];

        // Posição inicial no centro do jogador
        t->x = player->x + player->size / 2.0f;
        t->y = player->y + player->size / 2.0f;

        // Calcula a direção do disparo
        float dx = mouse->x - t->x;
        float dy = mouse->y - t->y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (distancia > 0.0f) {
            t->vel_x = (dx / distancia) * 8.0f;
            t->vel_y = (dy / distancia) * 8.0f;
        }
        else {
            t->vel_x = 0.0f;
            t->vel_y = 0.0f;
        }

        // Define a aparência padrão branca
        t->formato = FORMATO_CIRCULO;
        t->cor_r = 255;
        t->cor_g = 255;
        t->cor_b = 255;
        t->size = 2;

        // Copia a aparência da combinação química
        if (arma != TIRO_PADRAO) {
            t->formato = sq->formato_ativo;
            t->cor_r = sq->cor_r;
            t->cor_g = sq->cor_g;
            t->cor_b = sq->cor_b;
            t->size = sq->tamanho_ativo;

            // Evita tamanho inválido
            if (t->size <= 0) {
                t->size = 2;
            }
        }

        // Ativa o tiro por último
        t->ativo = true;

        break;
    }
}
