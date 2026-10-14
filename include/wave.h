#ifndef WAVE_H
#define WAVE_H

#include <stdbool.h>
#include <inimigos.h>

typedef struct {
	int atual;
	bool inicio;
	bool andamento;
	bool finalizado;
} Wave;

void init_wave(Wave* wave);
void sistema_waves(Wave* wave, Inimigo* qtd_inimigos[], int total_inimigos, Player* player);


#endif