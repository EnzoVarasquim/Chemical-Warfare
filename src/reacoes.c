/*

#include "reacoes.h"

void inicializar_sistema_quimico(SistemaQuimico* sq) {
    sq->qtd_H = 0;
    sq->qtd_Cl = 0;
    sq->qtd_Na = 0;
    sq->arma_ativa = TIRO_PADRAO;
}

void checar_reacoes(SistemaQuimico* sq) {
    //ÁCIDA (HCl)
    if (sq->qtd_H >= 1 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_ACIDA;
        sq->qtd_H -= 1;
        sq->qtd_Cl -= 1;
    }
    //BÁSICA (NaOH)
    else if (sq->qtd_Na >= 1 && sq->qtd_H >= 1) {
        sq->arma_ativa = ARMA_BASICA;
        sq->qtd_Na -= 1;
        sq->qtd_H -= 1;
    }
    //METÁLICA (NaCl)
    else if (sq->qtd_Na >= 2 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_METALICA;
        sq->qtd_Na -= 2;
        sq->qtd_Cl -= 1;
    }
}*/

#include "reacoes.h"
#include <allegro5/allegro_primitives.h>

void inicializar_sistema_quimico(SistemaQuimico* sq) {
    sq->qtd_H = 0;
    sq->qtd_Cl = 0;
    sq->qtd_Na = 0;

    // Aparência padrão
    sq->arma_ativa = TIRO_PADRAO;
    sq->formato_ativo = FORMATO_CIRCULO;
    sq->cor_r = 255;
    sq->cor_g = 255;
    sq->cor_b = 255;
    sq->tamanho_ativo = 2;
}

void checar_reacoes(SistemaQuimico* sq) {

    // ÁCIDA (HCl) - Triângulo verde
    if (sq->qtd_H >= 1 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_ACIDA;

        sq->formato_ativo = FORMATO_TRIANGULO;
        sq->cor_r = 80;
        sq->cor_g = 255;
        sq->cor_b = 80;
        sq->tamanho_ativo = 5;

        sq->qtd_H -= 1;
        sq->qtd_Cl -= 1;
    }

    // BÁSICA (NaOH) - Quadrado azul
    else if (sq->qtd_Na >= 1 && sq->qtd_H >= 1) {
        sq->arma_ativa = ARMA_BASICA;

        sq->formato_ativo = FORMATO_QUADRADO;
        sq->cor_r = 80;
        sq->cor_g = 180;
        sq->cor_b = 255;
        sq->tamanho_ativo = 7;

        sq->qtd_Na -= 1;
        sq->qtd_H -= 1;
    }

    // METÁLICA (NaCl) - Círculo prateado
    else if (sq->qtd_Na >= 2 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_METALICA;

        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 190;
        sq->cor_g = 190;
        sq->cor_b = 210;
        sq->tamanho_ativo = 3;

        sq->qtd_Na -= 2;
        sq->qtd_Cl -= 1;
    }

    // Sem reação: volta ao tiro padrão branco
    else {
        sq->arma_ativa = TIRO_PADRAO;
        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 255;
        sq->cor_g = 255;
        sq->cor_b = 255;
        sq->tamanho_ativo = 2;
    }
}