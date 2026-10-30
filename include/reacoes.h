#ifndef REACOES_H
#define REACOES_H

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

    // Aparencia da combinacao ativa
    FormatoTiro formato_ativo;
    int cor_r;
    int cor_g;
    int cor_b;
    float tamanho_ativo;

} SistemaQuimico;

void inicializar_sistema_quimico(SistemaQuimico* sq);
void checar_reacoes(SistemaQuimico* sq);

#endif
