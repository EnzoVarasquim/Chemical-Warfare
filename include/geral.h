#ifndef GERAL_H
#define GERAL_H

#include <allegro5/allegro_font.h>
#include "player.h"
#include "inimigos.h"
#include "tiro.h"
#include "wave.h"

void desenhar_background(int width, int height);
void desenhar_icones_quimica(int width, int height);
void desenhar_barra_lateral(int width, int height);
void desenhar_tutorial(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_sub, ALLEGRO_FONT* fonte_txt);
void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[], Wave* wave, int width, int height);

#endif
