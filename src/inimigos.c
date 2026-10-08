#include <allegro5/allegro_primitives.h>
#include "inimigos.h"

#define MAX_TIROS 30

Inimigo init_inimigo(int x, int y, TipoInimigo tipo) {
	Inimigo inimigo = { x, y, 20, 2, 10, true, tipo };
	return inimigo;
}

void desenhar_inimigo(Inimigo inimigo) {
    if (!inimigo.vivo) return;

    ALLEGRO_COLOR cor;
    switch (inimigo.tipo) {
    case TIPO_ACIDO:
        cor = al_map_rgb(0, 255, 0);       //Verde pra Ácido
        break;
    case TIPO_BASICO:
        cor = al_map_rgb(255, 0, 255);     //Magenta pra Base
        break;
    case TIPO_METALICO:
        cor = al_map_rgb(100, 100, 100);   //Cinza pra Metal
        break;
    default:
        cor = al_map_rgb(255, 0, 0);       // Vermelho caso dê um bug :P
        break;
    }

    al_draw_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 255, 255), 2);
    al_draw_filled_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, cor);
}

void seguir_player(Inimigo* inimigo, Player* player) {
    if (!inimigo->vivo || !player->vivo) return;

    float velocidade = inimigo->vel * 0.6f;

    if (inimigo->x < player->x + player->size)     //direita
        inimigo->x += velocidade;

    if (inimigo->x + inimigo->size > player->x)    //esquerda
        inimigo->x -= velocidade;

    if (inimigo->y + inimigo->size > player->y)    //cima
        inimigo->y -= velocidade;

    if (inimigo->y < player->y + player->size)     //baixo
        inimigo->y += velocidade;
}

//colisão entre inimigos
void colisao_inimigos(Inimigo* inimigo1, Inimigo* inimigo2) {
    if (!inimigo1->vivo || !inimigo2->vivo) return;

    //checa colisao
    if (inimigo1->x < inimigo2->x + inimigo2->size &&
        inimigo1->x + inimigo1->size > inimigo2->x &&
        inimigo1->y < inimigo2->y + inimigo2->size &&
        inimigo1->y + inimigo1->size > inimigo2->y)
    {
        //eles se empurram quando colidem no eixo X 
        if (inimigo1->x < inimigo2->x) {
            inimigo1->x -= 3; 
            inimigo2->x += 3; 
        }                  
        else {             
            inimigo1->x += 3; 
            inimigo2->x -= 3; 
        }

        //empurram quando colidem no eixo Y 
        if (inimigo1->y < inimigo2->y) {
            inimigo1->y -= 3; 
            inimigo2->y += 3; 
        }                  
        else {             
            inimigo1->y += 3; 
            inimigo2->y -= 3; 
        }
    }
}

//colisão do player com o inimigo
void colisao_player(Inimigo* inimigo, Player* player) {
    if (!inimigo->vivo || !player->vivo) return;

    //checa colisao
    if (inimigo->x < player->x + player->size &&     //direita
        inimigo->x + inimigo->size > player->x &&    //esquerda
        inimigo->y < player->y + player->size &&     //baixo
        inimigo->y + inimigo->size > player->y)      //cima
    {
        //dano no player quando toca nele
        player->vida--;

        if (player->vida <= 0) {
            player->vivo = false;
            return;
        }

        //faz com q o inimigo n entre dentro do player
        if (inimigo->x == player->x) {
            inimigo->x -= 3; //previni um bug dele ficar parado no lugar sem fazer nada
        }
        else if (inimigo->x < player->x) {
            inimigo->x -= 3; 
        }
        else {
            inimigo->x += 3;
        }

        if (inimigo->y == player->y) {
            inimigo->y -= 3; //previni um bug dele ficar parado no lugar sem fazer nada
        }
        else if (inimigo->y < player->y) {
            inimigo->y -= 1;
        }
        else {
            inimigo->y += 1;
        }
    }
}

//colissão do tiro com o inimigo
void colisao_tiro(Inimigo* inimigo, Tiro tiros[], SistemaQuimico* sq) {
    if (!inimigo->vivo) return;

    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            if (tiros[i].x + tiros[i].size >= inimigo->x &&
                tiros[i].x <= inimigo->x + inimigo->size &&
                tiros[i].y + tiros[i].size >= inimigo->y &&
                tiros[i].y <= inimigo->y + inimigo->size)
            {
                tiros[i].ativo = false;
                inimigo->vida--;

                if (inimigo->vida <= 0) {
                    inimigo->vida = 0;
                    inimigo->vivo = false;

                    // Adiciona direto no inventário numérico
                    if (inimigo->tipo == TIPO_ACIDO) sq->qtd_H++;
                    else if (inimigo->tipo == TIPO_BASICO) sq->qtd_Na++;
                    else if (inimigo->tipo == TIPO_METALICO) sq->qtd_Cl++;

                    checar_reacoes(sq);
                    break;
                }
            }
        }
    }
}

void colisao_tela_inimigo(Inimigo* inimigo) {
    if (inimigo->x < 0) {               //esquerda
        inimigo->x = 0;
    }

    if (inimigo->x > (640 - inimigo->size)) {       //direita
        inimigo->x = (640 - inimigo->size);
    }

    if (inimigo->y < 0) {               //cima
        inimigo->y = 0;
    }

    if (inimigo->y > (480 - inimigo->size)) {       //baixo
        inimigo->y = (480 - inimigo->size);
    }
}