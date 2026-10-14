
#ifndef TIRO_H
#define TIRO_H

#include <stdbool.h>
#include "reacoes.h"
#include "player.h"

#define MAX_TIROS 30

typedef struct {
    float x, y;
    float vel_x, vel_y;
    float size;

    int dano;
    int penetracao;

    TipoArma tipo;
    bool ativo;
} Tiro;

typedef struct {
    int x, y;
    bool left_button;
} Mouse;

Mouse init_mouse(void);
Tiro init_tiro(void);

void desenhar_tiros(Tiro tiros[]);
void desenhar_mira(Mouse* mouse);

void atualizar_posicao_tiros(Tiro tiros[], int largura_tela, int altura_tela);

void disparar_tiro(Mouse* mouse, Player* player, Tiro tiros[], TipoArma arma);

#endif
