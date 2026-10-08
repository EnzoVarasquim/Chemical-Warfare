#ifndef INIMIGOS_H
#define INIMIGOS_H

#include <stdbool.h>
#include "player.h"
#include "tiro.h"
#include "reacoes.h"

typedef enum {
    TIPO_ACIDO,
    TIPO_BASICO,
    TIPO_METALICO
} TipoInimigo;

typedef struct {
    int x;
    int y;
    int size;
    int vel;
    int vida;
    bool vivo;
    TipoInimigo tipo;
} Inimigo;

Inimigo init_inimigo(int x, int y, TipoInimigo tipo);
void desenhar_inimigo(Inimigo inimigo);
void colisao_tiro(Inimigo* inimigo, Tiro tiros[], SistemaQuimico* sq);
void seguir_player(Inimigo* inimigo, Player* player);
void colisao_inimigos(Inimigo* inimigo1, Inimigo* inimigo2);
void colisao_player(Inimigo* inimigo, Player* player);
void colisao_tela_inimigo(Inimigo* inimigo);

#endif