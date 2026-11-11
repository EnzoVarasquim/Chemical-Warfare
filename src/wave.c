#include "wave.h"
#include <stdlib.h> 
#include <math.h>

void init_wave(Wave* wave) {
    wave->atual = 1;
    wave->final = 21;            //fazendo os calculos a quantidade real de waves é 12 :)
    wave->inicio = true;
    wave->andamento = false;
    wave->finalizado = false;
    wave->vitoria = false;
    wave->laboratorio = false;
    wave->combates_vencidos = 0;
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
            //acessa o array original
            if ((*qtd_inimigos)[i].vivo) {
                vivos++;
            }
        }

        //se nenhum estiver vivo, a wave acaba
        if (vivos == 0 && wave->laboratorio == false) {
            //se for a wave final, o jogo acaba
            if (wave->atual >= wave->final + 1) {
                wave->vitoria = true;
                wave->andamento = false;
                wave->finalizado = false;
                return;
            }

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
        //Wave final (BOSS) - Intocada e isolada
        else if (wave->atual >= wave->final) {
            wave->laboratorio = false;

            *total_inimigos = 1;
            sq->research_point += 25;

            //redimensiona o array para caber só 1 inimigo
            *qtd_inimigos = (Inimigo*)realloc(*qtd_inimigos, (*total_inimigos) * sizeof(Inimigo));

            //nasce o Boss bem no centro da tela
            int centroX = width / 1.625;
            int centroY = height / 2;

            (*qtd_inimigos)[0] = init_inimigo(centroX - 30, centroY - 30, TIPO_BOSS);

        }
        //Waves normais (Garante pelo menos 2 Atiradores e o resto dos inimigos comuns)
        else {
            wave->laboratorio = false;

            (*total_inimigos)++;
            sq->research_point += 10;

            //realoca a memória do array original para caber o novo inimigo extra
            *qtd_inimigos = (Inimigo*)realloc(*qtd_inimigos, (*total_inimigos) * sizeof(Inimigo));

            // Garante que pelo menos os primeiros 2 inimigos da wave são Atiradores (se houver espaço, senão ajusta)
            int num_atiradores = (*total_inimigos >= 2) ? 2 : *total_inimigos;

            //spawna todos os inimigos
            for (int i = 0; i < *total_inimigos; i++) {
                int posX, posY;
                float distancia;

                do {
                    posX = (width / 4 + 20) + (rand() % (width - width / 4 - 2 * 20 + 1));
                    posY = 20 + (rand() % (height - 2 * 20 + 1));

                    float diffX = posX - player->x;
                    float diffY = posY - player->y;
                    distancia = sqrtf(diffX * diffX + diffY * diffY);

                } while (distancia < 75.0f);

                TipoInimigo tipo_escolhido;

                // Os primeiros 2 inimigos são obrigatoriamente Atiradores
                if (i < num_atiradores) {
                    tipo_escolhido = TIPO_ATIRADOR;
                }
                else {
                    // Os restantes são sorteados entre os tipos comuns (Ácido, Básico, Metálico)
                    tipo_escolhido = (TipoInimigo)(rand() % 3);
                }

                (*qtd_inimigos)[i] = init_inimigo(posX, posY, tipo_escolhido);
            }
        }

        //atualiza os estados para iniciar a próxima wave
        wave->atual++;
        wave->andamento = true;
        wave->finalizado = false;
    }
}   