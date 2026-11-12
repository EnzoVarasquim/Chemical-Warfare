#ifndef BOSS_H
#define BOSS_H

#include <stdbool.h>
#include "inimigos.h"
#include "player.h"

#define MAX_TIROS_BOSS 180

typedef struct {
    float x;
    float y;
    float vel_x;
    float vel_y;
    bool ativo;
} TiroBoss;

typedef struct {
    float angulo;
    double proximo_disparo;
    double proximo_dano;

    bool angulo_iniciado;
    bool fase2_ativada;

    TiroBoss tiros[MAX_TIROS_BOSS];
} AtaqueBoss;

void init_ataque_boss(AtaqueBoss* ataque);
void atualizar_ataque_boss(AtaqueBoss* ataque, Inimigo* boss, Player* player, int width, int height);
void desenhar_ataque_boss(AtaqueBoss* ataque, Inimigo* boss);
void colisao_ataque_boss(AtaqueBoss* ataque, Inimigo* boss, Player* player);

#endif