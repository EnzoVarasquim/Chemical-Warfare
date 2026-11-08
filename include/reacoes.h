#ifndef REACOES_H
#define REACOES_H

#include <allegro5/allegro_font.h>
#include "player.h"

typedef enum {
    ELEMENTO_NENHUM,
    ELEMENTO_H,
    ELEMENTO_CL,
    ELEMENTO_NA
} TipoElemento;

typedef enum {
    TIRO_PADRAO,
    ARMA_ACIDA,
    ARMA_BASICA,
    ARMA_METALICA
} TipoArma;

typedef enum {
    FORMATO_CIRCULO,
    FORMATO_QUADRADO,
    FORMATO_TRIANGULO
} FormatoTiro;

typedef struct {
    int qtd_H;
    int qtd_Cl;
    int qtd_Na;
    int research_point;
    TipoArma arma_ativa;

    FormatoTiro formato_ativo;
    int cor_r;
    int cor_g;
    int cor_b;
    float tamanho_ativo;

} SistemaQuimico;

typedef struct {
    int ID_item;
    const char* nome;
    const char* desc;
    int valor;
} ItemLab;

void inicializar_sistema_quimico(SistemaQuimico* sq);
void checar_reacoes(SistemaQuimico* sq);

void init_itens_lab(ItemLab* item);
void desenhar_itens_lab(SistemaQuimico* sq, ItemLab* item, ALLEGRO_FONT* fonte);
int verificar_colisao_loja(Player* player, int* item_posX, int* item_posY, int tamanho_caixa);
void computar_compra_item(SistemaQuimico* sq, Player* p, ItemLab* itens, int width, int height);

#endif