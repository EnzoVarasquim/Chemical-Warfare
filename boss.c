
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "boss.h"

// configuracoes do boss

#define PI 3.14159265f

#define VIDA_FASE_2 75

#define VELOCIDADE_TIRO_BOSS 5.0f
#define VELOCIDADE_TIRO_FASE_1 7.0f

#define RAIO_TIRO 6.0f

// configuracoes da fase 1

#define INTERVALO_FASE_1 0.25
#define ALCANCE_CONE 320.0f
#define ABERTURA_CONE (PI / 5.0f)

// configuracoes da fase 2

#define INTERVALO_FASE_2 0.16
#define ROTACAO_FASE_2 0.22f
#define TIROS_RADIAIS_FASE_2 8

// centraliza o boss na tela, iniciar a segunda fase

static void centralizar_boss(Inimigo* boss, int width, int height)
{
    float limite_esquerdo = width / 4.0f;

    boss->x = (int)(limite_esquerdo + (width - limite_esquerdo - boss->size) / 2.0f);
    boss->y = (int)((height - boss->size) / 2.0f);
}

// inicializa o sistema de ataque do boss

void init_ataque_boss(AtaqueBoss* ataque)
{
    ataque->angulo = 0.0f;
    ataque->angulo_visao = 0.0f;

    ataque->proximo_disparo = 0.0;
    ataque->proximo_dano = 0.0;

    ataque->angulo_iniciado = false;
    ataque->fase2_ativada = false;

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        ataque->tiros[i].ativo = false;
    }
}

// cria um tiro do boss

static void criar_tiro(AtaqueBoss* ataque, float x, float y, float angulo, float velocidade)
{
    for (int i = 0; i < MAX_TIROS_BOSS; i++) {
        if (!ataque->tiros[i].ativo) {

            ataque->tiros[i].x = x;
            ataque->tiros[i].y = y;

            ataque->tiros[i].vel_x = cosf(angulo) * velocidade;
            ataque->tiros[i].vel_y = sinf(angulo) * velocidade;

            ataque->tiros[i].ativo = true;

            return;
        }
    }
}

// cria um tiro na frente do boss

static void criar_tiro_na_frente(AtaqueBoss* ataque, Inimigo* boss, float angulo, float velocidade)
{
    float centro_x = boss->x + boss->size / 2.0f;
    float centro_y = boss->y + boss->size / 2.0f;

    float distancia_frente = boss->size / 2.0f + RAIO_TIRO + 2.0f;

    float tiro_x = centro_x + cosf(angulo) * distancia_frente;
    float tiro_y = centro_y + sinf(angulo) * distancia_frente;

    criar_tiro(ataque, tiro_x, tiro_y, angulo, velocidade);
}

// movimento dos projeteis do boss

static void atualizar_projeteis(AtaqueBoss* ataque, int width, int height)
{
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

// verifica se o player esta dentro do cone

static bool player_dentro_cone(AtaqueBoss* ataque, Inimigo* boss, Player* player)
{
    float centro_x = boss->x + boss->size / 2.0f;
    float centro_y = boss->y + boss->size / 2.0f;

    float player_x = player->x + player->size / 2.0f;
    float player_y = player->y + player->size / 2.0f;

    float dx = player_x - centro_x;
    float dy = player_y - centro_y;

    float distancia_quadrada = dx * dx + dy * dy;

    if (distancia_quadrada > ALCANCE_CONE * ALCANCE_CONE)
        return false;

    float angulo_player = atan2f(dy, dx);

    float diferenca = atan2f(
        sinf(angulo_player - ataque->angulo_visao),
        cosf(angulo_player - ataque->angulo_visao)
    );

    return fabsf(diferenca) <= ABERTURA_CONE / 2.0f;
}

// atualiza os ataques da fase 1

static void atualizar_fase1(AtaqueBoss* ataque, Inimigo* boss, Player* player, double agora)
{
    float centro_x = boss->x + boss->size / 2.0f;
    float centro_y = boss->y + boss->size / 2.0f;

    float player_x = player->x + player->size / 2.0f;
    float player_y = player->y + player->size / 2.0f;

    float dx = player_x - centro_x;
    float dy = player_y - centro_y;

    ataque->angulo_visao = atan2f(dy, dx);

    if (player_dentro_cone(ataque, boss, player) &&
        agora >= ataque->proximo_disparo) {

        criar_tiro_na_frente(
            ataque,
            boss,
            ataque->angulo_visao,
            VELOCIDADE_TIRO_FASE_1
        );

        ataque->proximo_disparo = agora + INTERVALO_FASE_1;
    }
}

// atualiza o ataque do boss

void atualizar_ataque_boss(AtaqueBoss* ataque, Inimigo* boss, Player* player, int width, int height)
{
    if (!boss->vivo || !player->vivo || boss->tipo != TIPO_BOSS)
        return;

    double agora = al_get_time();

    // fase 1

    if (boss->vida > VIDA_FASE_2) {

        atualizar_fase1(ataque, boss, player, agora);

        atualizar_projeteis(ataque, width, height);

        return;
    }

    // fase 2

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

            criar_tiro_na_frente(
                ataque,
                boss,
                angulo,
                VELOCIDADE_TIRO_BOSS
            );
        }

        ataque->angulo += ROTACAO_FASE_2;

        if (ataque->angulo >= 2.0f * PI)
            ataque->angulo -= 2.0f * PI;

        ataque->proximo_disparo = agora + INTERVALO_FASE_2;
    }

    atualizar_projeteis(ataque, width, height);
}

// desenha o ataque do boss

void desenhar_ataque_boss(AtaqueBoss* ataque, Inimigo* boss)
{
    if (!boss->vivo || boss->tipo != TIPO_BOSS)
        return;

    // cone transparente com contorno branco

    if (boss->vida > VIDA_FASE_2) {

        float centro_x = boss->x + boss->size / 2.0f;
        float centro_y = boss->y + boss->size / 2.0f;

        al_draw_pieslice(
            centro_x,
            centro_y,
            ALCANCE_CONE,
            ataque->angulo_visao - ABERTURA_CONE / 2.0f,
            ABERTURA_CONE,
            al_map_rgba(255, 255, 255, 150),
            1.5f
        );
    }

    // cor dos projeteis

    ALLEGRO_COLOR cor = al_map_rgb(80, 190, 255);

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {

        if (!ataque->tiros[i].ativo)
            continue;

        float x = ataque->tiros[i].x;
        float y = ataque->tiros[i].y;

        al_draw_filled_circle(x, y, RAIO_TIRO, cor);

        al_draw_circle(
            x,
            y,
            RAIO_TIRO,
            al_map_rgb(255, 255, 255),
            1
        );
    }
}

// colisao do ataque do boss com o player

void colisao_ataque_boss(AtaqueBoss* ataque, Inimigo* boss, Player* player)
{
    if (!boss->vivo || !player->vivo || boss->tipo != TIPO_BOSS)
        return;

    double agora = al_get_time();

    bool pode_danar = agora >= ataque->proximo_dano;

    bool acertou = false;

    for (int i = 0; i < MAX_TIROS_BOSS; i++) {

        if (!ataque->tiros[i].ativo)
            continue;

        float x = ataque->tiros[i].x;
        float y = ataque->tiros[i].y;

        float px = fmaxf(player->x, fminf(x, player->x + player->size));
        float py = fmaxf(player->y, fminf(y, player->y + player->size));

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

        if (player->vida <= 0)
            player->vivo = false;

        ataque->proximo_dano = agora + 0.5;
    }
}
