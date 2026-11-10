#ifndef GERAL_H
#define GERAL_H

#include <allegro5/allegro_font.h>
#include "player.h"
#include "inimigos.h"
#include "tiro.h"

void desenhar_background(int width, int height);
void desenhar_icones_quimica(int width, int height);
void desenhar_barra_lateral(int width, int height);
void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[], int width, int height);

#endif
