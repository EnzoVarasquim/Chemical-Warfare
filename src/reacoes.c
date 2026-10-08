#include "reacoes.h"

void inicializar_sistema_quimico(SistemaQuimico* sq) {
    sq->qtd_H = 0;
    sq->qtd_Cl = 0;
    sq->qtd_Na = 0;
    sq->arma_ativa = TIRO_PADRAO;
}

void checar_reacoes(SistemaQuimico* sq) {
    //REAÇÃO ÁCIDA: H + Cl = HCl
    if (sq->qtd_H >= 1 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_ACIDA;
        sq->qtd_H -= 1;
        sq->qtd_Cl -= 1;
    }
    //REAÇÃO BÁSICA: Na + H + Cl (simplificado para balancear o Sódio/Hidrogênio)
    else if (sq->qtd_Na >= 1 && sq->qtd_H >= 1) {
        sq->arma_ativa = ARMA_BASICA;
        sq->qtd_Na -= 1;
        sq->qtd_H -= 1;
    }
    //REAÇÃO METÁLICA/SALINA: Na + Cl = NaCl
    else if (sq->qtd_Na >= 1 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_METALICA;
        sq->qtd_Na -= 1;
        sq->qtd_Cl -= 1;
    }
}