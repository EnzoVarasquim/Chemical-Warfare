
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

    // Tiro padrao branco
    tiro.cor_r = 255;
    tiro.cor_g = 255;
    tiro.cor_b = 255;

    tiro.vel_x = 0;
    tiro.vel_y = 0;

    return tiro;
}

void desenhar_tiros(Tiro tiros[]) {
    for (int i = 0; i < MAX_TIROS; i++) {

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

        case FORMATO_TRIANGULO: {
            // Angulo da direcao do disparo
            float angulo = atan2f(t->vel_y, t->vel_x);

            // Tamanho do triangulo
            float tamanho = t->size * 1.2f;

            // Ponta do triangulo
            float frente_x = t->x + cosf(angulo) * tamanho;
            float frente_y = t->y + sinf(angulo) * tamanho;

            // Parte traseira
            float tras_x = t->x - cosf(angulo) * tamanho * 0.7f;
            float tras_y = t->y - sinf(angulo) * tamanho * 0.7f;

            // Largura do triangulo
            float lado_x = sinf(angulo) * tamanho * 0.6f;
            float lado_y = -cosf(angulo) * tamanho * 0.6f;

            al_draw_filled_triangle(
                frente_x, frente_y,
                tras_x + lado_x, tras_y + lado_y,
                tras_x - lado_x, tras_y - lado_y,
                cor
            );
            break;
        }

        default:
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

        // Desativa quando sai da tela
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

        if (tiros[i].ativo) {
            continue;
        }

        Tiro* t = &tiros[i];

        // Posicao inicial no centro do jogador
        t->x = player->x + player->size / 2.0f;
        t->y = player->y + player->size / 2.0f;

        // Calcula a direcao do disparo
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

        // Tiro padrao branco
        t->formato = FORMATO_CIRCULO;
        t->cor_r = 255;
        t->cor_g = 255;
        t->cor_b = 255;
        t->size = 2;

        // Aparencia das armas quimicas
        if (arma != TIRO_PADRAO) {

            t->formato = sq->formato_ativo;
            t->cor_r = sq->cor_r;
            t->cor_g = sq->cor_g;
            t->cor_b = sq->cor_b;
            t->size = sq->tamanho_ativo;

            // Circulos quimicos roxos
            if (t->formato == FORMATO_CIRCULO) {
                t->cor_r = 160;
                t->cor_g = 32;
                t->cor_b = 240;
            }

            // Quadrados menores
            if (t->formato == FORMATO_QUADRADO) {
                t->size *= 0.5f;
            }

            // Triangulos menores
            if (t->formato == FORMATO_TRIANGULO) {
                t->size *= 0.5f;
            }

            // Evita tamanho invalido
            if (t->size <= 0) {
                t->size = 2;
            }
        }

        // Ativa o tiro por ultimo
        t->ativo = true;

        break;
    }
}
