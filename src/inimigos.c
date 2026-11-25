#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "inimigos.h"

#define MAX_TIROS 30

Inimigo init_inimigo(int x, int y, TipoInimigo tipo) {
    int tamanho = 20;
    float velocidade = 2.0f;
    int vida = 5;

    if (tipo == TIPO_ACIDO) {
        tamanho = 20;
        velocidade = 3;
        vida = 5;
    }
    if (tipo == TIPO_BASICO) {
        tamanho = 20;
        velocidade = 2.5;
        vida = 7;
    }
    if (tipo == TIPO_METALICO) {
        tamanho = 20;
        velocidade = 2;
        vida = 10;
    }

    if (tipo == TIPO_BOSS) {
        tamanho = 60;
        velocidade = 1;
        vida = 150;
    }

    Inimigo inimigo = { x, y, tamanho, velocidade, vida, true, false, tipo };
    return inimigo;
}

void desenhar_inimigo(Inimigo inimigo) {
    if (!inimigo.vivo) return;

    ALLEGRO_COLOR cor;
    switch (inimigo.tipo) {
    case TIPO_ACIDO:
        cor = al_map_rgb(0, 255, 0);       // Verde pra Ácido
        break;
    case TIPO_BASICO:
        cor = al_map_rgb(255, 0, 255);     // Magenta pra Base
        break;
    case TIPO_METALICO:
        cor = al_map_rgb(100, 100, 100);   // Cinza pra Metal
        break;
    case TIPO_BOSS:
        cor = al_map_rgb(255, 69, 0);      // Laranja Escuro pro Boss
        break;
    default:
        cor = al_map_rgb(255, 255, 255);   // Branco caso dê bug
        break;
    }

    al_draw_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 255, 255), 2);
    al_draw_filled_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, cor);
}

void seguir_player(Inimigo* inimigo, Player* player) {
    if (!inimigo->vivo || !player->vivo) return;

    float velocidade = inimigo->vel;

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
        //dá dano no player quando toca nele
        tomar_dano(player, 1);

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
void colisao_tiro(Inimigo* inimigo, Tiro tiros[], SistemaQuimico* sq, Player* player) {
    if (!inimigo->vivo) return;

    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            if (tiros[i].x + tiros[i].size >= inimigo->x &&
                tiros[i].x <= inimigo->x + inimigo->size &&
                tiros[i].y + tiros[i].size >= inimigo->y &&
                tiros[i].y <= inimigo->y + inimigo->size)
            {
                tiros[i].ativo = false;

                int dano_final = player->dano;

                //interações diferentes dependendo do tipo de tiro que colidindo com um tipo de inimigo
                if (sq->arma_ativa == ARMA_ACIDA) {
                    if (inimigo->tipo == TIPO_METALICO) dano_final *= 2;
                    if (inimigo->tipo == TIPO_ACIDO) dano_final = 0;      //imune
                }
                if (sq->arma_ativa == ARMA_BASICA) {
                    if (inimigo->tipo == TIPO_ACIDO) dano_final *= 2;
                    if (inimigo->tipo == TIPO_BASICO) dano_final = 0;     //imune
                }
                if (sq->arma_ativa == ARMA_METALICA) {
                    if (inimigo->tipo == TIPO_BASICO) dano_final *= 2;
                    if (inimigo->tipo == TIPO_ACIDO || inimigo->tipo == TIPO_METALICO) dano_final = 0;      //imune
                }

                inimigo->vida -= dano_final;

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

void colisao_tela_inimigo(Inimigo* inimigo, int width, int height) {
    if (inimigo->x < width / 4) {                       //esquerda
        inimigo->x = width / 4;
    }

    if (inimigo->x > (width - inimigo->size)) {       //direita
        inimigo->x = (width - inimigo->size);
    }

    if (inimigo->y < 0) {                              //cima
        inimigo->y = 0;
    }

    if (inimigo->y > (height - inimigo->size)) {       //baixo
        inimigo->y = (height - inimigo->size);
    }
}

//gerar a moeda (research point)
void atualizar_moedas(Inimigo inimigos[], int total, Player* player, SistemaQuimico* sq) {
    for (int i = 0; i < total; i++) {
        if (!inimigos[i].vivo && inimigos[i].vida != -99 && player->vivo) {
            if (player->x < inimigos[i].x + inimigos[i].size &&
                player->x + player->size > inimigos[i].x &&
                player->y < inimigos[i].y + inimigos[i].size &&
                player->y + player->size > inimigos[i].y)
            {
                inimigos[i].vida = -99; //marca como recolhido para sumir

                //add research point para cada tipo de inimigo morto e a moeda for coletada
                if (inimigos[i].tipo == TIPO_ACIDO) sq->research_point++;
                else if (inimigos[i].tipo == TIPO_BASICO) sq->research_point++;
                else if (inimigos[i].tipo == TIPO_METALICO) sq->research_point++;
            }
        }
    }
}

void desenhar_moedas_mortos(Inimigo inimigos[], int total) {
    for (int i = 0; i < total; i++) {
        if (!inimigos[i].vivo && inimigos[i].vida != -99) {
            //ifs ternários pra verificar a cor da moedinha para o respectivo tipo de inimigo
            ALLEGRO_COLOR cor = (inimigos[i].tipo == TIPO_ACIDO) ? al_map_rgb(0, 255, 0) :
                (inimigos[i].tipo == TIPO_BASICO) ? al_map_rgb(255, 0, 255) :
                (inimigos[i].tipo == TIPO_METALICO) ? al_map_rgb(100, 100, 100) :
                al_map_rgb(255, 255, 255);

            //centro dos inimigos
            float cx = inimigos[i].x + inimigos[i].size / 2;
            float cy = inimigos[i].y + inimigos[i].size / 2;
            float s = 12;   //tamanho do triângulo

            //desenha a moeda na tela
            al_draw_filled_triangle(cx, cy - s / 2, cx - s / 2, cy + s / 2, cx + s / 2, cy + s / 2, cor);
            al_draw_triangle(cx, cy - s / 2, cx - s / 2, cy + s / 2, cx + s / 2, cy + s / 2, al_map_rgb(255, 255, 255), 1);
        }
    }
}

// Disparos aleatorios do boss
void disparar_tiro_boss(Inimigo* boss, Tiro tiros_boss[]) {

    if (!boss->vivo || boss->tipo != TIPO_BOSS) return;

    static double ultimo_disparo = 0;

    double agora = al_get_time();

    // Intervalo de 0.05 segundos
    if (agora - ultimo_disparo < 0.05) return;

    for (int i = 0; i < MAX_TIROS; i++) {

        if (!tiros_boss[i].ativo) {

            Tiro* t = &tiros_boss[i];

            // Posicao inicial no centro do boss
            t->x = boss->x + boss->size / 2;
            t->y = boss->y + boss->size / 2;

            // Direcao aleatoria
            float angulo = ((float)rand() / RAND_MAX) * 2.0f * 3.14159265f;

            // Velocidade baixa
            t->vel_x = cosf(angulo) * 10.0f;
            t->vel_y = sinf(angulo) * 10.0f;

            // Aparencia do projetil
            t->size = 5;
            t->formato = FORMATO_CIRCULO;

            t->cor_r = 255;
            t->cor_g = 69;
            t->cor_b = 0;

            t->ativo = true;

            ultimo_disparo = agora;
            break;
        }
    }
}

// Colisao do tiro do boss com o jogador
void colisao_tiro_boss(Tiro tiros_boss[], Player* player) {

    if (!player->vivo) return;

    for (int i = 0; i < MAX_TIROS; i++) {

        if (!tiros_boss[i].ativo) continue;

        if (tiros_boss[i].x + tiros_boss[i].size >= player->x &&
            tiros_boss[i].x - tiros_boss[i].size <= player->x + player->size &&
            tiros_boss[i].y + tiros_boss[i].size >= player->y &&
            tiros_boss[i].y - tiros_boss[i].size <= player->y + player->size) {

            tiros_boss[i].ativo = false;

            tomar_dano(player, 1);

            if (player->vida <= 0) {
                player->vivo = false;
                return;
            }
        }
    }
}

static void desenhar_forma_estrela(float cx, float cy, float raio_ext, float raio_int, float rotacao, ALLEGRO_COLOR cor_fill, ALLEGRO_COLOR cor_borda) {
    float vertices[20];
    for (int i = 0; i < 10; i++) {
        float r = (i % 2 == 0) ? raio_ext : raio_int;
        float angulo = rotacao + (i * ALLEGRO_PI / 5.0f) - (ALLEGRO_PI / 2.0f);
        vertices[i * 2] = cx + cosf(angulo) * r;
        vertices[i * 2 + 1] = cy + sinf(angulo) * r;
    }
    al_draw_filled_polygon(vertices, 10, cor_fill);
    al_draw_polygon(vertices, 10, ALLEGRO_LINE_JOIN_ROUND, cor_borda, 1.5f, 0.0f);
}

void tentar_dropar_estrela(EstrelaBonus* estrela, Inimigo* inimigo) {
    if (!estrela->ativa && !estrela->coletada) {
        if (rand() % 30 == 0) {
            estrela->x = inimigo->x + inimigo->size / 2.0f;
            estrela->y = inimigo->y + inimigo->size / 2.0f;
            estrela->raio = 12.0f;
            estrela->ativa = true;
            estrela->coletada = false;
            estrela->valor_research = 5;
        }
    }
}

void atualizar_estrela(EstrelaBonus* estrela, Player* player, SistemaQuimico* sq) {
    if (!estrela->ativa || estrela->coletada || !player->vivo) return;

    float hitbox = estrela->raio;
    if (player->x < estrela->x + hitbox &&
        player->x + player->size > estrela->x - hitbox &&
        player->y < estrela->y + hitbox &&
        player->y + player->size > estrela->y - hitbox) {

        estrela->ativa = false;
        estrela->coletada = true;
        sq->research_point += estrela->valor_research;
    }
}

void desenhar_estrela(EstrelaBonus estrela) {
    if (!estrela.ativa || estrela.coletada) return;

    double tempo = al_get_time();

    ALLEGRO_COLOR palheta[6] = {
        al_map_rgb(255, 230, 0),
        al_map_rgb(255, 50, 50),
        al_map_rgb(50, 255, 100),
        al_map_rgb(50, 180, 255),
        al_map_rgb(255, 80, 255),
        al_map_rgb(255, 140, 0)
    };

    int indice_cor = ((int)(tempo * 12.0)) % 6;
    ALLEGRO_COLOR cor_atual = palheta[indice_cor];

    float pulso = 1.0f + 0.18f * sinf((float)(tempo * 8.0));
    float rotacao = (float)(tempo * 2.5);

    desenhar_forma_estrela(
        estrela.x,
        estrela.y,
        estrela.raio * pulso,
        (estrela.raio * 0.45f) * pulso,
        rotacao,
        cor_atual,
        al_map_rgb(255, 255, 255)
    );
}