#include "wave.h"
#include <stdlib.h> 
#include <math.h>

void init_wave(Wave* wave) {
    wave->atual = 1;        
    wave->inicio = true;    
    wave->andamento = false;
    wave->finalizado = false;
}

void sistema_waves(Wave* wave, Inimigo** qtd_inimigos, int* total_inimigos, Player* player) {
    if (wave->inicio == true) {
        wave->andamento = true;
        wave->inicio = false;
    }

    //checa se todos os inimigos ativos morreram
    if (wave->andamento == true) {
        int vivos = 0;
        for (int i = 0; i < *total_inimigos; i++) {
            //acessa o array original usando (*qtd_inimigos)
            if ((*qtd_inimigos)[i].vivo) {
                vivos++;
            }
        }

        //se nenhum estiver vivo, a wave foi finalizada
        if (vivos == 0) {
            wave->finalizado = true;
            wave->andamento = false;
        }
    }

    if (wave->finalizado == true) {

        //aumenta o número total de inimigos na memória da main
        (*total_inimigos)++;

        //realoca a memória do array original para caber o novo inimigo extra
        *qtd_inimigos = (Inimigo*)realloc(*qtd_inimigos, (*total_inimigos) * sizeof(Inimigo));

        //spawna todos os inimigos
        for (int i = 0; i < *total_inimigos; i++) {
            int posX, posY;
            float distancia;

            do {
                posX = 20 + (rand() % (640 - 2 * 20 + 1));
                posY = 20 + (rand() % (480 - 2 * 20 + 1));

                float diffX = posX - player->x;
                float diffY = posY - player->y;
                distancia = sqrtf(diffX * diffX + diffY * diffY);

            } while (distancia < 75.0f);

            TipoInimigo tipo_aleatorio = (TipoInimigo)(rand() % 3);
            (*qtd_inimigos)[i] = init_inimigo(posX, posY, tipo_aleatorio);
        }

        //atualiza os estados para iniciar a próxima wave
        wave->atual++;
        wave->andamento = true;
        wave->finalizado = false;
    }
}