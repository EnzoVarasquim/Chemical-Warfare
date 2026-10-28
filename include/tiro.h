
#ifndef TIRO_H
#define TIRO_H

#include <stdbool.h>
#include "player.h"
#include "reacoes.h"

#define MAX_TIROS 30

typedef struct {
    int x;
    int y;
    float size;
    float vel_x;
    float vel_y;
    bool ativo;

    FormatoTiro formato;
    int cor_r;
    int cor_g;
    int cor_b;
} Tiro;

typedef struct {
    int x;
    int y;
    bool left_button;
} Mouse;

Mouse init_mouse(void);
Tiro init_tiro(void);

void desenhar_tiros(Tiro tiros[]);
void desenhar_mira(Mouse* mouse);

void atualizar_posicao_tiros(Tiro tiros[], int largura_tela, int altura_tela);

void disparar_tiro(Mouse* mouse, Player* player, Tiro tiros[], TipoArma arma, SistemaQuimico* sq);

#endif
