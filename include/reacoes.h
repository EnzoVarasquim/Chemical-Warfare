#ifndef REACOES_H
#define REACOES_H

#include <stdbool.h>

typedef enum {
    ELEMENTO_NENHUM,
    ELEMENTO_H,    // Hidrogênio (Essencial para o Ácido)
    ELEMENTO_CL,   // Cloro
    ELEMENTO_NA    // Sódio (Metal)
} TipoElemento;

typedef enum {
    TIRO_PADRAO,
    ARMA_ACIDA,    // Reação HCl (Ácido Clorídrico)
    ARMA_BASICA,   // Reação NaOH (Hidróxido de Sódio)
    ARMA_METALICA  // Reação NaCl (Cloreto de Sódio - Sal/Metal)
} TipoArma;

// Mantemos as structs simples de inventário
typedef struct {
    int qtd_H;
    int qtd_Cl;
    int qtd_Na;
    TipoArma arma_ativa;
} SistemaQuimico;

// Adicione as assinaturas das novas funções
void inicializar_sistema_quimico(SistemaQuimico* sq);
void checar_reacoes(SistemaQuimico* sq);

#endif
