#ifndef GERAL_H
#define GERAL_H

#include "player.h"
#include "inimigos.h"
#include "tiro.h"

void desenhar_background(void);
void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[]);

#endif
