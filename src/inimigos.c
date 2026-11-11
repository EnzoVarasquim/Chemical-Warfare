#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "inimigos.h"
#include <stdlib.h>

#define MAX_TIROS 30

Inimigo init_inimigo(int x, int y, TipoInimigo tipo) {
    int tamanho = 20, velocidade = 2, vida = 5;

    if (tipo == TIPO_ACIDO) {
        tamanho = 20;
        velocidade = 3;
        vida = 5;
    }
    else if (tipo == TIPO_BASICO) {
        tamanho = 20;
        velocidade = 2.5;
        vida = 7;
    }
    else if (tipo == TIPO_METALICO) {
        tamanho = 20;
        velocidade = 2;
        vida = 10;
    }
    else if (tipo == TIPO_BOSS) {
        tamanho = 60;
        velocidade = 1;
        vida = 150;
    }
    else if (tipo == TIPO_ATIRADOR) {
        tamanho = 20;
        velocidade = 0; // Fica estático
        vida = 5;
    }

    Inimigo inimigo = { x, y, tamanho, velocidade, vida, true, false, tipo };
    return inimigo;
}

void desenhar_inimigo(Inimigo inimigo) {
    if (!inimigo.vivo) return;

    ALLEGRO_COLOR cor;
    switch (inimigo.tipo) {
    case TIPO_ACIDO:
        cor = al_map_rgb(0, 255, 0);
        break;
    case TIPO_BASICO:
        cor = al_map_rgb(255, 0, 255);
        break;
    case TIPO_METALICO:
        cor = al_map_rgb(100, 100, 100);
        break;
    case TIPO_BOSS:
        cor = al_map_rgb(255, 69, 0);
        break;
    case TIPO_ATIRADOR:
        cor = al_map_rgb(0, 191, 255);      // Azul Céu para o Atirador
        break;
    default:
        cor = al_map_rgb(255, 255, 255);
        break;
    }

    al_draw_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, al_map_rgb(255, 255, 255), 2);
    al_draw_filled_rectangle(inimigo.x, inimigo.y, inimigo.x + inimigo.size, inimigo.y + inimigo.size, cor);
}

void seguir_player(Inimigo* inimigo, Player* player) {
    if (!inimigo->vivo || !player->vivo) return;
    if (inimigo->tipo == TIPO_ATIRADOR) return; // Atirador fica parado

    float velocidade = inimigo->vel;

    if (inimigo->x < player->x + player->size)
        inimigo->x += velocidade;

    if (inimigo->x + inimigo->size > player->x)
        inimigo->x -= velocidade;

    if (inimigo->y + inimigo->size > player->y)
        inimigo->y -= velocidade;

    if (inimigo->y < player->y + player->size)
        inimigo->y += velocidade;
}

void colisao_inimigos(Inimigo* inimigo1, Inimigo* inimigo2) {
    if (!inimigo1->vivo || !inimigo2->vivo) return;

    if (inimigo1->x < inimigo2->x + inimigo2->size &&
        inimigo1->x + inimigo1->size > inimigo2->x &&
        inimigo1->y < inimigo2->y + inimigo2->size &&
        inimigo1->y + inimigo1->size > inimigo2->y)
    {
        if (inimigo1->x < inimigo2->x) {
            inimigo1->x -= 3;
            inimigo2->x += 3;
        }
        else {
            inimigo1->x += 3;
            inimigo2->x -= 3;
        }

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

void colisao_player(Inimigo* inimigo, Player* player) {
    if (!inimigo->vivo || !player->vivo) return;

    if (inimigo->x < player->x + player->size &&
        inimigo->x + inimigo->size > player->x &&
        inimigo->y < player->y + player->size &&
        inimigo->y + inimigo->size > player->y)
    {
        tomar_dano(player, 1);

        if (player->vida <= 0) {
            player->vivo = false;
            return;
        }

        if (inimigo->x == player->x) {
            inimigo->x -= 3;
        }
        else if (inimigo->x < player->x) {
            inimigo->x -= 3;
        }
        else {
            inimigo->x += 3;
        }

        if (inimigo->y == player->y) {
            inimigo->y -= 3;
        }
        else if (inimigo->y < player->y) {
            inimigo->y -= 1;
        }
        else {
            inimigo->y += 1;
        }
    }
}

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
                inimigo->vida -= player->dano;

                if (inimigo->vida <= 0) {
                    inimigo->vida = 0;
                    inimigo->vivo = false;

                    if (inimigo->tipo == TIPO_ACIDO) sq->qtd_H++;
                    else if (inimigo->tipo == TIPO_BASICO) sq->qtd_Na++;
                    else if (inimigo->tipo == TIPO_METALICO) sq->qtd_Cl++;
                    else if (inimigo->tipo == TIPO_ATIRADOR) sq->qtd_H++;

                    checar_reacoes(sq);
                    break;
                }
            }
        }
    }
}

void colisao_tela_inimigo(Inimigo* inimigo, int width, int height) {
    if (inimigo->x < width / 4) {
        inimigo->x = width / 4;
    }

    if (inimigo->x > (width - inimigo->size)) {
        inimigo->x = (width - inimigo->size);
    }

    if (inimigo->y < 0) {
        inimigo->y = 0;
    }

    if (inimigo->y > (height - inimigo->size)) {
        inimigo->y = (height - inimigo->size);
    }
}

void atualizar_moedas(Inimigo inimigos[], int total, Player* player, SistemaQuimico* sq) {
    for (int i = 0; i < total; i++) {
        if (!inimigos[i].vivo && inimigos[i].vida != -99 && player->vivo) {
            if (player->x < inimigos[i].x + inimigos[i].size &&
                player->x + player->size > inimigos[i].x &&
                player->y < inimigos[i].y + inimigos[i].size &&
                player->y + player->size > inimigos[i].y)
            {
                inimigos[i].vida = -99;

                if (inimigos[i].tipo == TIPO_ACIDO) sq->research_point++;
                else if (inimigos[i].tipo == TIPO_BASICO) sq->research_point++;
                else if (inimigos[i].tipo == TIPO_METALICO) sq->research_point++;
                else if (inimigos[i].tipo == TIPO_ATIRADOR) sq->research_point++;
            }
        }
    }
}

void desenhar_moedas_mortos(Inimigo inimigos[], int total) {
    for (int i = 0; i < total; i++) {
        if (!inimigos[i].vivo && inimigos[i].vida != -99) {
            ALLEGRO_COLOR cor = (inimigos[i].tipo == TIPO_ACIDO) ? al_map_rgb(0, 255, 0) :
                (inimigos[i].tipo == TIPO_BASICO) ? al_map_rgb(255, 0, 255) :
                (inimigos[i].tipo == TIPO_METALICO) ? al_map_rgb(100, 100, 100) :
                al_map_rgb(255, 255, 255);

            float cx = inimigos[i].x + inimigos[i].size / 2;
            float cy = inimigos[i].y + inimigos[i].size / 2;
            float s = 12;

            al_draw_filled_triangle(cx, cy - s / 2, cx - s / 2, cy + s / 2, cx + s / 2, cy + s / 2, cor);
            al_draw_triangle(cx, cy - s / 2, cx - s / 2, cy + s / 2, cx + s / 2, cy + s / 2, al_map_rgb(255, 255, 255), 1);
        }
    }
}

// Disparos do boss (100% intocado)
void disparar_tiro_boss(Inimigo* boss, Tiro tiros_boss[]) {
    if (!boss->vivo || boss->tipo != TIPO_BOSS) return;

    static double ultimo_disparo = 0;
    double agora = al_get_time();

    if (agora - ultimo_disparo < 0.05) return;

    for (int i = 0; i < MAX_TIROS; i++) {
        if (!tiros_boss[i].ativo) {
            Tiro* t = &tiros_boss[i];

            t->x = boss->x + boss->size / 2;
            t->y = boss->y + boss->size / 2;

            float angulo = ((float)rand() / RAND_MAX) * 2.0f * 3.14159265f;

            t->vel_x = cosf(angulo) * 10.0f;
            t->vel_y = sinf(angulo) * 10.0f;

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

// Colisao do tiro do boss com o jogador (intocado)
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

// Disparos do Atirador (Agora aponta e atira diretamente no player)
void disparar_tiro_atirador(Inimigo* inimigo, Player* player, Tiro tiros[]) {
    if (!inimigo->vivo || !player->vivo || inimigo->tipo != TIPO_ATIRADOR) return;

    static double tempo_ultimo_tiro = 0.0;

    if (al_get_time() - tempo_ultimo_tiro >= 1.5) { // Dispara a cada 1.5 segundos
        for (int i = 0; i < MAX_TIROS; i++) {
            if (!tiros[i].ativo) {
                tiros[i].x = inimigo->x + inimigo->size / 2;
                tiros[i].y = inimigo->y + inimigo->size / 2;
                tiros[i].ativo = true;

                // Calcula a direção exata para o player
                float dx = (player->x + player->size / 2) - tiros[i].x;
                float dy = (player->y + player->size / 2) - tiros[i].y;
                float distancia = sqrtf(dx * dx + dy * dy);

                if (distancia > 0) {
                    tiros[i].vel_x = (dx / distancia) * 4.0f;
                    tiros[i].vel_y = (dy / distancia) * 4.0f;
                }
                else {
                    tiros[i].vel_x = 0.0f;
                    tiros[i].vel_y = 4.0f;
                }

                tiros[i].formato = FORMATO_CIRCULO;
                tiros[i].cor_r = 0;
                tiros[i].cor_g = 191;
                tiros[i].cor_b = 255;
                tiros[i].size = 4;

                tempo_ultimo_tiro = al_get_time();
                break;
            }
        }
    }
}