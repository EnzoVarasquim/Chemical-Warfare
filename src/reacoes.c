#include "reacoes.h"
#include <allegro5/allegro_primitives.h>

void inicializar_sistema_quimico(SistemaQuimico* sq) {

    sq->qtd_H = 0;
    sq->qtd_Cl = 0;
    sq->qtd_Na = 0;
    sq->research_point = 0;

    // Arma inicial
    sq->arma_ativa = TIRO_PADRAO;

    // Aparência padrão
    sq->formato_ativo = FORMATO_CIRCULO;
    sq->cor_r = 255;
    sq->cor_g = 255;
    sq->cor_b = 255;
    sq->tamanho_ativo = 2;
    sq->arma_desbloqueada = TIRO_PADRAO;
    sq->tempo_mensagem_desbloqueio = 0;

    // Desbloqueios
    sq->acida_desbloqueada = false;
    sq->basica_desbloqueada = false;
    sq->metalica_desbloqueada = false;

    sq->mensagem_arma_bloqueada = false;
}


// Atualiza a aparência conforme a arma selecionada
void atualizar_aparencia_arma(SistemaQuimico* sq) {

    switch (sq->arma_ativa) {

    case TIRO_PADRAO:
        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 255;
        sq->cor_g = 255;
        sq->cor_b = 255;
        sq->tamanho_ativo = 2;
        break;

    case ARMA_ACIDA:
        sq->formato_ativo = FORMATO_TRIANGULO;
        sq->cor_r = 80;
        sq->cor_g = 255;
        sq->cor_b = 80;
        sq->tamanho_ativo = 5;
        break;

    case ARMA_BASICA:
        sq->formato_ativo = FORMATO_QUADRADO;
        sq->cor_r = 80;
        sq->cor_g = 180;
        sq->cor_b = 255;
        sq->tamanho_ativo = 7;
        break;

    case ARMA_METALICA:
        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 190;
        sq->cor_g = 190;
        sq->cor_b = 210;
        sq->tamanho_ativo = 3;
        break;
    }
}


// Verifica se o jogador pode selecionar a arma
bool selecionar_arma(SistemaQuimico* sq, TipoArma arma) {

    bool desbloqueada = false;

    switch (arma) {

    case TIRO_PADRAO:
        desbloqueada = true;
        break;

    case ARMA_ACIDA:
        desbloqueada = sq->acida_desbloqueada;
        break;

    case ARMA_BASICA:
        desbloqueada = sq->basica_desbloqueada;
        break;

    case ARMA_METALICA:
        desbloqueada = sq->metalica_desbloqueada;
        break;
    }

    if (desbloqueada) {

        sq->arma_ativa = arma;
        sq->mensagem_arma_bloqueada = false;

        atualizar_aparencia_arma(sq);

        return true;
    }

    sq->mensagem_arma_bloqueada = true;

    return false;
}


// Verifica as combinações químicas e desbloqueia armas
void checar_reacoes(SistemaQuimico* sq) {

    // ÁCIDA (HCl)
    if (sq->qtd_H >= 1 && sq->qtd_Cl >= 1) {

        sq->acida_desbloqueada = true;

        sq->qtd_H -= 1;
        sq->qtd_Cl -= 1;
    }

    // BÁSICA (NaOH)
    else if (sq->qtd_Na >= 1 && sq->qtd_H >= 1) {

        sq->basica_desbloqueada = true;

        sq->qtd_Na -= 1;
        sq->qtd_H -= 1;
    }

    // METÁLICA (NaCl)
    else if (sq->qtd_Na >= 2 && sq->qtd_Cl >= 1) {

        sq->metalica_desbloqueada = true;

        sq->qtd_Na -= 2;
        sq->qtd_Cl -= 1;
    }
}