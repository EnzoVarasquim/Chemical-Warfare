#ifndef WAVE_H
#define WAVE_H

#include <stdbool.h>
#include "inimigos.h"
#include "player.h"

typedef struct {
    int atual;
    int final;
    bool inicio;
    bool andamento;
    bool finalizado;
    bool vitoria;
} Wave;

void init_wave(Wave* wave);
void sistema_waves(Wave* wave, Inimigo** qtd_inimigos, int* total_inimigos, Player* player);

#endif