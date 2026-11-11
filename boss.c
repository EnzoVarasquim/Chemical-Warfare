#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "boss.h"

#define PI 3.14159265f

#define VIDA_FASE_2 75

#define VELOCIDADE_TIRO_BOSS 4.0f
#define RAIO_TIRO 6.0f

#define INTERVALO_FASE_2 0.16
#define ROTACAO_FASE_2 0.22f
#define TIROS_RADIAIS_FASE_2 8

static void centralizar_boss(Inimigo* boss, int width, int height)
{
    float limite_esquerdo = width / 4.0f;

    boss->x = (int)(limite_esquerdo +
        (width - limite_esquerdo - boss->size) / 2.0f);

    boss->y = (int)((height - boss->size) / 2.0f);
}

void init_ataque_boss(AtaqueBoss* ataque)
{
    ataque->angulo = 0.0f;
    ataque->proximo_disparo = 0.0;
    ataque->proximo_dano = 0.0;

    ataque->angulo_iniciado = false;
    ataque->fase2_ativada = false;

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        ataque->tiros[i].ativo = false;
    }
}

static void criar_tiro(
    AtaqueBoss* ataque,
    float x,
    float y,
    float angulo
) {
    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        if (!ataque->tiros[i].ativo) {
            ataque->tiros[i].x = x;
            ataque->tiros[i].y = y;

            ataque->tiros[i].vel_x =
                cosf(angulo) * VELOCIDADE_TIRO_BOSS;

            ataque->tiros[i].vel_y =
                sinf(angulo) * VELOCIDADE_TIRO_BOSS;

            ataque->tiros[i].ativo = true;
            return;
        }
    }
}

static void atualizar_projeteis(
    AtaqueBoss* ataque,
    int width,
    int height
) {
    float limite_esquerdo = width / 4.0f;

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        if (!ataque->tiros[i].ativo)
            continue;

        ataque->tiros[i].x += ataque->tiros[i].vel_x;
        ataque->tiros[i].y += ataque->tiros[i].vel_y;

        float x = ataque->tiros[i].x;
        float y = ataque->tiros[i].y;

        if (x - RAIO_TIRO <= limite_esquerdo ||
            x + RAIO_TIRO >= width ||
            y - RAIO_TIRO <= 0 ||
            y + RAIO_TIRO >= height) {
            ataque->tiros[i].ativo = false;
        }
    }
}

void atualizar_ataque_boss(
    AtaqueBoss* ataque,
    Inimigo* boss,
    Player* player,
    int width,
    int height
) {
    if (!boss->vivo ||
        !player->vivo ||
        boss->tipo != TIPO_BOSS) {
        return;
    }

    double agora = al_get_time();

    // PRIMEIRA FASE: o chefe persegue o jogador.
    // O movimento é controlado pelo main.c; não há disparos nesta fase.
    if (boss->vida > VIDA_FASE_2) {
        return;
    }

    // SEGUNDA FASE: o chefe fica centralizado e dispara em todas as direções.
    centralizar_boss(boss, width, height);

    float centro_x = boss->x + boss->size / 2.0f;
    float centro_y = boss->y + boss->size / 2.0f;

    if (!ataque->fase2_ativada) {
        ataque->fase2_ativada = true;

        for (int i = 0; i < MAX_TIROS_BOSS; i++) {
            ataque->tiros[i].ativo = false;
        }

        ataque->proximo_disparo = agora;
    }

    if (agora >= ataque->proximo_disparo) {
        for (int i = 0; i < TIROS_RADIAIS_FASE_2; i++) {
            float angulo = ataque->angulo +
                i * (2.0f * PI / TIROS_RADIAIS_FASE_2);

            criar_tiro(ataque, centro_x, centro_y, angulo);
        }

        ataque->angulo += ROTACAO_FASE_2;

        if (ataque->angulo >= 2.0f * PI) {
            ataque->angulo -= 2.0f * PI;
        }

        ataque->proximo_disparo = agora + INTERVALO_FASE_2;
    }

    atualizar_projeteis(ataque, width, height);
}

void desenhar_ataque_boss(
    AtaqueBoss* ataque,
    Inimigo* boss
) {
    if (!boss->vivo || boss->tipo != TIPO_BOSS)
        return;

    ALLEGRO_COLOR cor;

    if (boss->vida > VIDA_FASE_2) {
        return;
    }

    cor = al_map_rgb(80, 190, 255);

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        if (!ataque->tiros[i].ativo)
            continue;

        float x = ataque->tiros[i].x;
        float y = ataque->tiros[i].y;

        al_draw_filled_circle(x, y, RAIO_TIRO, cor);
        al_draw_circle(
            x, y, RAIO_TIRO,
            al_map_rgb(255, 255, 255), 1
        );
    }
}

void colisao_ataque_boss(
    AtaqueBoss* ataque,
    Inimigo* boss,
    Player* player
) {
    if (!boss->vivo ||
        !player->vivo ||
        boss->tipo != TIPO_BOSS) {
        return;
    }

    double agora = al_get_time();

    bool pode_danar = agora >= ataque->proximo_dano;
    bool acertou = false;

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        if (!ataque->tiros[i].ativo)
            continue;

        float x = ataque->tiros[i].x;
        float y = ataque->tiros[i].y;

        float px = fmaxf(
            player->x,
            fminf(x, player->x + player->size)
        );

        float py = fmaxf(
            player->y,
            fminf(y, player->y + player->size)
        );

        float dx = x - px;
        float dy = y - py;

        if (dx * dx + dy * dy <= RAIO_TIRO * RAIO_TIRO) {
            ataque->tiros[i].ativo = false;

            if (pode_danar) {
                acertou = true;
                break;
            }
        }
    }

    if (acertou) {
        tomar_dano(player, 1);

        if (player->vida <= 0) {
            player->vivo = false;
        }

        ataque->proximo_dano = agora + 0.5;
    }
}
