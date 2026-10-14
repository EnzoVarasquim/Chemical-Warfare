#include "wave.h"
#include <math.h>

void init_wave(Wave* wave) {
	wave->atual = 0;
	wave->inicio = false;
	wave->andamento = false;
	wave->finalizado = false;
}

void sistema_waves(Wave* wave, Inimigo* qtd_inimigos[], int total_inimigos, Player* player) {
    if (wave->inicio == true) {
        wave->atual = 1;
        wave->andamento == true;
        !wave->inicio;
    }
    
    if (wave->andamento == true) {
        if (total_inimigos <= 0) {
            wave->finalizado == true;
            !wave->andamento;
        }
    }

	if (wave->finalizado == true) {

        //spawna os inimigos de novo com um numero maior
        total_inimigos++;

        for (int i = 0; i < total_inimigos; i++) {
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
            qtd_inimigos[i] = init_inimigo(posX, posY, tipo_aleatorio);
        }

        //troca o estado da wave e aumenta o numero da wave atual
		wave->atual++;
        wave->andamento == true;
        !wave->finalizado;
	}
}