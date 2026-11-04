#ifndef GERAL_H
#define GERAL_H

#include "player.h"
#include "inimigos.h"
#include "tiro.h"

void desenhar_background(int width, int height);
void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[], int width, int height);

#endif
