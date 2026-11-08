#ifndef REACOES_H
#define REACOES_H

#include <stdbool.h>

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

void inicializar_sistema_quimico(SistemaQuimico* sq);
void checar_reacoes(SistemaQuimico* sq);
bool selecionar_arma(SistemaQuimico* sq, TipoArma arma);
void atualizar_aparencia_arma(SistemaQuimico* sq);



#endif