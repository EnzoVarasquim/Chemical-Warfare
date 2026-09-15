#ifndef INIMIGOS_H
#define INIMIGOS_H

#include <stdbool.h>
#include "tiro.h"
#include "player.h"

typedef struct {
    int x;
    int y;
    int size;
    int vel;
    int vida;
    bool vivo;
} Inimigo;

Inimigo init_inimigo(int x, int y);
void desenhar_inimigo(Inimigo inimigo);
void colisao_tiro(Inimigo* inimigo, Tiro tiros[]);
void seguir_player(Inimigo* inimigo, int player_x, int player_y);
void colisao_inimigos(Inimigo* inimigo1, Inimigo* inimigo2);
void colisao_player(Inimigo* inimigo, Player* player);

#endif