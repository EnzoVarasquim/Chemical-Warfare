#ifndef WAVE_H
#define WAVE_H

#include <stdbool.h>
#include "inimigos.h"
#include "player.h"
#include "reacoes.h"

#define TEMPO_PAUSA_WAVE 3.0

typedef struct {
    int atual;
    int final;
    bool inicio;
    bool andamento;
    bool finalizado;
    bool vitoria;
    bool laboratorio;
    int combates_vencidos;
    bool em_pausa;
    double tempo_inicio_pausa;
} Wave;

void init_wave(Wave* wave);
void sistema_waves(Wave* wave, Inimigo** qtd_inimigos, int* total_inimigos, Player* player, SistemaQuimico* sq, int width, int height);

#endif