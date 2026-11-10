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

    // Aparência da arma
    FormatoTiro formato_ativo;
    int cor_r;
    int cor_g;
    int cor_b;
    float tamanho_ativo;

    // Armas desbloqueadas
    bool acida_desbloqueada;
    bool basica_desbloqueada;
    bool metalica_desbloqueada;

    // Mensagem de seleção inválida
    bool mensagem_arma_bloqueada;

    // Aviso de nova arma
    TipoArma arma_desbloqueada;
    int tempo_mensagem_desbloqueio;
    
} SistemaQuimico;

typedef struct {
    int ID_item;
    const char* nome;
    const char* desc;
    int valor;
    int contador_compra;
} ItemLab;

void inicializar_sistema_quimico(SistemaQuimico* sq);
void checar_reacoes(SistemaQuimico* sq);
bool selecionar_arma(SistemaQuimico* sq, TipoArma arma);
void atualizar_aparencia_arma(SistemaQuimico* sq);

void init_itens_lab(ItemLab* item);
void desenhar_itens_lab(SistemaQuimico* sq, ItemLab* item, ALLEGRO_FONT* fonte);
int verificar_colisao_loja(Player* player, int* item_posX, int* item_posY, int tamanho_caixa);
void computar_compra_item(SistemaQuimico* sq, Player* player, ItemLab* itens, int width, int height);

#endif