#ifndef GERAL_H
#define GERAL_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "player.h"
#include "inimigos.h"
#include "tiro.h"
#include "wave.h"

#define TOP_RANKING 10

typedef struct {
    char nome[4];
    int score;
} RegistroRanking;

void desenhar_background(int width, int height);
void desenhar_icones_quimica(int width, int height);
void desenhar_barra_lateral(int width, int height);
void desenhar_tutorial(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_sub, ALLEGRO_FONT* fonte_txt);

void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[], Wave* wave, int width, int height);

void desenhar_popup(int width, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_dica, const char* titulo, const char* dica, ALLEGRO_COLOR cor);
void desenhar_fim_de_jogo(int width, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, bool venceu, const char* nome, int score);
void desenhar_tela_nome(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_letra, ALLEGRO_FONT* fonte_texto, char nome[], int tamanho);
void desenhar_ranking(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, RegistroRanking ranking[], int total);

int calcular_score_final(Player* player, SistemaQuimico* sq);
void salvar_score(const char* nome, int score);
int carregar_ranking(RegistroRanking ranking[]);

#endif
