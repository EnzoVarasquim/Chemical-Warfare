
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "tiro.h"

// Inicializa o mouse
Mouse init_mouse(void) {
    Mouse mouse = { 0, 0, false };
    return mouse;
}

// Inicializa um tiro
Tiro init_tiro(void) {
    Tiro tiro = { 0 };

    tiro.size = 2;
    tiro.dano = 1;
    tiro.penetracao = 0;
    tiro.tipo = TIRO_PADRAO;
    tiro.ativo = false;

    return tiro;
}

// Desenha a mira do mouse
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

// Desenha todos os tiros ativos
void desenhar_tiros(Tiro tiros[]) {
    for (int i = 0; i < MAX_TIROS; i++) {
        if (!tiros[i].ativo) {
            continue;
        }

        ALLEGRO_COLOR cor;

        switch (tiros[i].tipo) {
        case ARMA_ACIDA:
            cor = al_map_rgb(27, 209, 161);
            break;

        case ARMA_BASICA:
            cor = al_map_rgb(80, 180, 255);
            break;

        case ARMA_METALICA:
            cor = al_map_rgb(204, 91, 16);
            break;

        case TIRO_PADRAO:
        default:
            cor = al_map_rgb(27, 94, 209);
            break;
        }

        al_draw_filled_circle(
            tiros[i].x,
            tiros[i].y,
            tiros[i].size,
            cor
        );
    }
}

// Atualiza a posição dos tiros
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

        // Desativa o tiro quando ele sai da tela
        if (tiros[i].x < 0 ||
            tiros[i].x > largura_tela ||
            tiros[i].y < 0 ||
            tiros[i].y > altura_tela) {

            tiros[i].ativo = false;
        }
    }
}

// Dispara um tiro em direção ao mouse
void disparar_tiro(
    Mouse* mouse,
    Player* player,
    Tiro tiros[],
    TipoArma arma
) {
    // Impede o disparo se o jogador estiver morto
    if (!player->vivo) {
        return;
    }

    // Só dispara enquanto o botão esquerdo estiver pressionado
    if (!mouse->left_button) {
        return;
    }

    // Procura um espaço livre no vetor de tiros
    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            continue;
        }

        // Inicializa o tiro
        tiros[i] = init_tiro();

        tiros[i].ativo = true;
        tiros[i].x = player->x;
        tiros[i].y = player->y;
        tiros[i].tipo = arma;

        // Define os atributos conforme a arma
        switch (arma) {
        case ARMA_ACIDA:
            tiros[i].dano = 2;
            tiros[i].size = 3;
            break;

        case ARMA_BASICA:
            tiros[i].dano = 1;
            tiros[i].size = 3;
            break;

        case ARMA_METALICA:
            tiros[i].dano = 2;
            tiros[i].penetracao = 2;
            tiros[i].size = 3;
            break;

        case TIRO_PADRAO:
        default:
            tiros[i].dano = 1;
            tiros[i].size = 2;
            break;
        }

        // Calcula a direção do tiro até o mouse
        float dx = mouse->x - player->x;
        float dy = mouse->y - player->y;

        float distancia = sqrtf(dx * dx + dy * dy);

        // Evita divisão por zero
        if (distancia == 0) {
            tiros[i].ativo = false;
            return;
        }

        // Define a velocidade e a direção
        float velocidade = 8.0f;

        tiros[i].vel_x = (dx / distancia) * velocidade;
        tiros[i].vel_y = (dy / distancia) * velocidade;

        // Dispara apenas um tiro por chamada
        break;
    }
}
