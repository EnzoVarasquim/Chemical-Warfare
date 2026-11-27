#include "wave.h"
#include <stdlib.h> 
#include <math.h>
#include <allegro5/allegro.h>

void init_wave(Wave* wave) {
    wave->atual = 1;
    wave->final = 21;            //fazendo os calculos a quantidade real de waves é 12 :)
    wave->inicio = true;
    wave->andamento = false;
    wave->finalizado = false;
    wave->vitoria = false;
    wave->laboratorio = false;
    wave->combates_vencidos = 0;
    wave->em_pausa = false;
    wave->tempo_inicio_pausa = 0.0;
}

void sistema_waves(Wave* wave, Inimigo** qtd_inimigos, int* total_inimigos, Player* player, SistemaQuimico* sq, int width, int height) {

    if (wave->vitoria == true) {
        return;
    }

    if (wave->inicio == true) {
        wave->andamento = true;
        wave->inicio = false;
    }

    //checa se todos os inimigos ativos morreram
    if (wave->andamento == true) {
        int vivos = 0;
        for (int i = 0; i < *total_inimigos; i++) {
            if ((*qtd_inimigos)[i].vivo) {
                vivos++;
            }
        }

        if (vivos == 0 && wave->laboratorio == false) {
            //se for a wave final, o jogo acaba (sem pausa)
            if (wave->atual >= wave->final + 1) {
                wave->vitoria = true;
                wave->andamento = false;
                wave->finalizado = false;
                return;
            }

            //primeiro tick sem inimigos: começa a pausa e espera
            if (wave->em_pausa == false) {
                wave->em_pausa = true;
                wave->tempo_inicio_pausa = al_get_time();
                return;
            }

            //pausa ainda não acabou: continua esperando
            if (al_get_time() - wave->tempo_inicio_pausa < TEMPO_PAUSA_WAVE) {
                return;
            }

            //pausa acabou: segue para a próxima wave
            wave->em_pausa = false;
            wave->combates_vencidos++;
            wave->atual++;
            wave->finalizado = true;
            wave->andamento = false;
        }
    }

    if (wave->finalizado == true) {
        //Wave do Laboratório (LOJA)
        if (wave->combates_vencidos == 3 && wave->atual != wave->final) {
            wave->laboratorio = true;
            *total_inimigos = 0;

            player->x = width / 1.6 - 10;
            player->y = height - 110;
        }
        //Wave final (BOSS)
        else if (wave->atual >= wave->final) {
            wave->laboratorio = false;

            Inimigo* temp = (Inimigo*)realloc(*qtd_inimigos, 1 * sizeof(Inimigo));
            if (temp == NULL) {
                return;
            }
            *qtd_inimigos = temp;
            *total_inimigos = 1;
            sq->research_point += 25;

            int centroX = width / 1.625;
            int centroY = height / 2;

            (*qtd_inimigos)[0] = init_inimigo(centroX - 30, centroY - 30, TIPO_BOSS);
        }
        //Waves normais
        else {
            wave->laboratorio = false;

            int novo_total = *total_inimigos + 1;
            Inimigo* temp = (Inimigo*)realloc(*qtd_inimigos, novo_total * sizeof(Inimigo));
            if (temp == NULL) {
                return;
            }
            *qtd_inimigos = temp;
            *total_inimigos = novo_total;
            sq->research_point += 10;

            int num_atiradores = (*total_inimigos >= 2) ? 2 : *total_inimigos;

            for (int i = 0; i < *total_inimigos; i++) {
                int posX = 0;
                int posY = 0;
                float distancia = 0.0f;

                do {
                    posX = (width / 4 + 20) + (rand() % (width - width / 4 - 2 * 20 + 1));
                    posY = 20 + (rand() % (height - 2 * 20 + 1));

                    float diffX = posX - player->x;
                    float diffY = posY - player->y;
                    distancia = sqrtf(diffX * diffX + diffY * diffY);

                } while (distancia < 75.0f);

                TipoInimigo tipo_escolhido;

                if (i < num_atiradores) {
                    tipo_escolhido = TIPO_ATIRADOR;
                }
                else {
                    tipo_escolhido = (TipoInimigo)(rand() % 3);
                }

                (*qtd_inimigos)[i] = init_inimigo(posX, posY, tipo_escolhido);
            }
        }

        wave->atual++;
        wave->andamento = true;
        wave->finalizado = false;
    }
}