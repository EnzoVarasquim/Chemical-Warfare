#ifndef INIMIGOS_H
#define INIMIGOS_H

#include <stdbool.h>
#include "player.h"
#include "tiro.h"
#include "reacoes.h"

typedef enum {
    TIPO_ACIDO,
    TIPO_BASICO,
    TIPO_METALICO,
    TIPO_BOSS
} TipoInimigo;

typedef struct {
    int x;
    int y;
    int size;
    int vel;
    int vida;
    bool vivo;
    bool coletado;
    TipoInimigo tipo;
} Inimigo;

Inimigo init_inimigo(int x, int y, TipoInimigo tipo);
void desenhar_inimigo(Inimigo inimigo);
void colisao_tiro(Inimigo* inimigo, Tiro tiros[], SistemaQuimico* sq);
void seguir_player(Inimigo* inimigo, Player* player);
void colisao_inimigos(Inimigo* inimigo1, Inimigo* inimigo2);
void colisao_player(Inimigo* inimigo, Player* player);
void colisao_tela_inimigo(Inimigo* inimigo, int width, int height);

void atualizar_moedas(Inimigo inimigos[], int total, Player* player, SistemaQuimico* sq);
void desenhar_moedas_mortos(Inimigo inimigos[], int total);

void disparar_tiro_boss(Inimigo* boss, Tiro tiros_boss[]);
void colisao_tiro_boss(Tiro tiros_boss[], Player* player);
#endif