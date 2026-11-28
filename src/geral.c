#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "geral.h"

void desenhar_background(int width, int height) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int x = width/4; x <= width; x += 20) {
        al_draw_line(x, 0, x, height, al_map_rgb(50, 50, 50), 1);
    }
    for (int y = 0; y <= height; y += 20) {
        al_draw_line(width / 4.0f, y, width, y, al_map_rgb(50, 50, 50), 1);
    }
}

void desenhar_icones_quimica(int width, int height) {
    al_draw_line(0, 330, width / 4.0f - 1, 330, al_map_rgb(50, 50, 50), 1);
}

void desenhar_barra_lateral(int width, int height) {
    al_draw_filled_rectangle(0, 0, width / 4.0f, height, al_map_rgb(10, 10, 10));
    al_draw_line(width / 4.0f - 1, 0, width / 4.0f - 1, height, al_map_rgb(50, 50, 50), 1);

    al_draw_line(0, 80, width / 4.0f - 1, 80, al_map_rgb(50, 50, 50), 1);
    al_draw_line(0, 115, width / 4.0f - 1, 115, al_map_rgb(50, 50, 50), 1);
    al_draw_line(0, 190, width / 4.0f - 1, 190, al_map_rgb(50, 50, 50), 1);

    desenhar_icones_quimica(width, height);

    al_draw_line(0, 460, width / 4.0f - 1, 460, al_map_rgb(50, 50, 50), 1);
}

void desenhar_tutorial(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_sub, ALLEGRO_FONT* fonte_txt) {
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cinza = al_map_rgb(90, 90, 90);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 235, 120);
    ALLEGRO_COLOR azul = al_map_rgb(0, 200, 255);

    int esq = 30;
    int dir = width / 2.0f + 20;

    al_draw_text(fonte_titulo, branco, width / 2.0f, 10, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
    al_draw_line(20, 46, width - 20, 46, cinza, 1);
    al_draw_line(width / 2.0f, 56, width / 2.0f, height - 40, cinza, 1);

    // ---------- COLUNA ESQUERDA ----------
    // a) Controles
    al_draw_text(fonte_sub, azul, esq, 56, ALLEGRO_ALIGN_LEFT, "CONTROLES");
    al_draw_text(fonte_txt, branco, esq, 80, ALLEGRO_ALIGN_LEFT, "WASD / Setas: mover");
    al_draw_text(fonte_txt, branco, esq, 96, ALLEGRO_ALIGN_LEFT, "Mouse: mirar | Botão esquerdo: atirar");
    al_draw_text(fonte_txt, branco, esq, 112, ALLEGRO_ALIGN_LEFT, "1 a 4: trocar de arma (se desbloqueada)");
    al_draw_text(fonte_txt, branco, esq, 128, ALLEGRO_ALIGN_LEFT, "R: reiniciar | ESC: sair do jogo");

    // b) Bestiário
    al_draw_text(fonte_sub, azul, esq, 156, ALLEGRO_ALIGN_LEFT, "INIMIGOS E DROPS");

    ALLEGRO_COLOR cores[5] = {
        al_map_rgb(0, 255, 0), al_map_rgb(255, 0, 255), al_map_rgb(100, 100, 100),
        al_map_rgb(0, 191, 255), al_map_rgb(255, 69, 0)
    };
    const char* textos[5] = {
        "Ácido: dropa H (hidrogênio)",
        "Básico: dropa Na (sódio)",
        "Metálico: dropa Cl (cloro)",
        "Atirador: dropa O, S ou C (aleatório)",
        "Boss: sem drop. Derrote para vencer!"
    };

    for (int i = 0; i < 5; i++) {
        int y = 182 + i * 22;
        al_draw_filled_rectangle(esq, y, esq + 14, y + 14, cores[i]);
        al_draw_rectangle(esq, y, esq + 14, y + 14, branco, 1);
        al_draw_text(fonte_txt, branco, esq + 24, y + 1, ALLEGRO_ALIGN_LEFT, textos[i]);
    }
    al_draw_text(fonte_txt, amarelo, esq, 296, ALLEGRO_ALIGN_LEFT, "O elemento só entra no inventário ao pegar a moeda!");

    // Neutralização
    al_draw_text(fonte_sub, azul, esq, 330, ALLEGRO_ALIGN_LEFT, "NEUTRALIZAÇÃO");
    al_draw_text(fonte_txt, branco, esq, 354, ALLEGRO_ALIGN_LEFT, "Ácido + Base -> Sal + Água");
    al_draw_text(fonte_txt, branco, esq, 370, ALLEGRO_ALIGN_LEFT, "HCl + NaOH -> NaCl + H2O");

    // Laboratório
    al_draw_text(fonte_sub, azul, esq, 404, ALLEGRO_ALIGN_LEFT, "LABORATÓRIO");
    al_draw_text(fonte_txt, branco, esq, 428, ALLEGRO_ALIGN_LEFT, "A cada 3 combates: encoste nos itens");
    al_draw_text(fonte_txt, branco, esq, 444, ALLEGRO_ALIGN_LEFT, "para comprar upgrades com Research Points.");

    // ---------- COLUNA DIREITA ----------
    // c) Reações
    al_draw_text(fonte_sub, azul, dir, 56, ALLEGRO_ALIGN_LEFT, "REAÇÕES (RECEITAS)");
    const char* receitas_txt[6] = {
        "HCl = H + Cl  ->  Arma Ácida (tecla 2)",
        "NaOH = Na + O + H  ->  Arma Básica (tecla 3)",
        "NaCl = Na + Cl  ->  Arma Metálica (tecla 4)",
        "H2O = 2H + O  ->  +1 vida",
        "CO2 = C + 2O  ->  +1 dano",
        "SO2 = S + 2O  ->  +1 velocidade"
    };
    for (int i = 0; i < 6; i++) {
        al_draw_text(fonte_txt, branco, dir, 80 + i * 20, ALLEGRO_ALIGN_LEFT, receitas_txt[i]);
    }
    al_draw_text(fonte_txt, amarelo, dir, 204, ALLEGRO_ALIGN_LEFT, "Arma já desbloqueada: refazer a reação dá +3 RP");

    // d) Vantagens químicas
    al_draw_text(fonte_sub, azul, dir, 236, ALLEGRO_ALIGN_LEFT, "VANTAGENS QUÍMICAS");

    const char* cab[3] = { "Ácido", "Básico", "Metálico" };
    const char* armas[4] = { "Ácida (HCl)", "Básica (NaOH)", "Metálica (NaCl)", "Padrão" };
    const char* efeito[4][3] = {
        { "0",  "1x", "2x" },
        { "2x", "0",  "1x" },
        { "0",  "2x", "0"  },
        { "1x", "1x", "1x" }
    };

    int x0 = dir, y0 = 262, larg0 = 120, larg = 70, alt = 24;

    al_draw_rectangle(x0, y0, x0 + larg0, y0 + alt, cinza, 1);
    al_draw_text(fonte_txt, branco, x0 + 6, y0 + 6, ALLEGRO_ALIGN_LEFT, "Arma \\ Inimigo");

    for (int c = 0; c < 3; c++) {
        int cx = x0 + larg0 + c * larg;
        al_draw_rectangle(cx, y0, cx + larg, y0 + alt, cinza, 1);
        al_draw_text(fonte_txt, cores[c], cx + larg / 2.0f, y0 + 6, ALLEGRO_ALIGN_CENTER, cab[c]);
    }

    for (int r = 0; r < 4; r++) {
        int y = y0 + alt * (r + 1);
        al_draw_rectangle(x0, y, x0 + larg0, y + alt, cinza, 1);
        al_draw_text(fonte_txt, branco, x0 + 6, y + 6, ALLEGRO_ALIGN_LEFT, armas[r]);

        for (int c = 0; c < 3; c++) {
            int cx = x0 + larg0 + c * larg;
            ALLEGRO_COLOR cor = branco;
            if (efeito[r][c][0] == '2') cor = al_map_rgb(80, 255, 80);
            if (efeito[r][c][0] == '0') cor = al_map_rgb(255, 80, 80);

            al_draw_rectangle(cx, y, cx + larg, y + alt, cinza, 1);
            al_draw_text(fonte_txt, cor, cx + larg / 2.0f, y + 6, ALLEGRO_ALIGN_CENTER, efeito[r][c]);
        }
    }

    al_draw_text(fonte_txt, branco, dir, 394, ALLEGRO_ALIGN_LEFT, "2x = dano dobrado | 0 = imune | 1x = normal");
    al_draw_text(fonte_txt, branco, dir, 410, ALLEGRO_ALIGN_LEFT, "Atirador e Boss recebem dano normal de todas.");

    al_draw_text(fonte_sub, amarelo, width / 2.0f, height - 30, ALLEGRO_ALIGN_CENTER, "Pressione ESC ou ENTER para voltar");
}

void reset(Player* player, Inimigo qtd_inimigos[], int total_inimigos, Tiro tiros[], Wave* wave, int width, int height) {
    *player = init_player(width, height);
    player->score = 0;
    player->combo = 1;

    for (int i = 0; i < total_inimigos; i++) {
        int tamanho_inimigo = 20;
        int posX, posY;
        float distancia;

        do {
            posX = (width / 4 + tamanho_inimigo) + (rand() % (width - width / 4 - 2 * tamanho_inimigo + 1));
            posY = tamanho_inimigo + (rand() % (height - 2 * tamanho_inimigo + 1));

            float diffX = posX - player->x;
            float diffY = posY - player->y;
            distancia = sqrtf(diffX * diffX + diffY * diffY);

        } while (distancia < 150.0f);

        TipoInimigo tipo;

        if (wave->atual >= wave->final + 1 && i == 0) {
            tipo = TIPO_BOSS;
        }
        else if (i == 0) {
            tipo = TIPO_ATIRADOR; // O primeiro inimigo das waves normais será o atirador
        }
        else {
            tipo = (TipoInimigo)(rand() % 3); // Os restantes continuam a ser os normais (Ácido, Básico, Metálico)
        }

        qtd_inimigos[i] = init_inimigo(posX, posY, tipo);
    }

    for (int i = 0; i < MAX_TIROS; i++) {
        tiros[i] = init_tiro();
    }
}

// Pop-up padrão (usado para arma desbloqueada e arma bloqueada)
void desenhar_popup(int width, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_dica, const char* titulo, const char* dica, ALLEGRO_COLOR cor) {
    float centro_x = width / 1.6f;

    al_draw_filled_rectangle(centro_x - 190, 120, centro_x + 190, 170, al_map_rgb(0, 0, 0));
    al_draw_rectangle(centro_x - 190, 120, centro_x + 190, 170, cor, 2);
    al_draw_text(fonte_titulo, cor, centro_x, 128, ALLEGRO_ALIGN_CENTER, titulo);
    al_draw_text(fonte_dica, al_map_rgb(255, 255, 255), centro_x, 150, ALLEGRO_ALIGN_CENTER, dica);
}

// Tela de Game Over / Vitória
void desenhar_fim_de_jogo(int width, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, bool venceu, const char* nome, int score) {
    float centro_x = width / 1.6f;
    ALLEGRO_COLOR cor_titulo = al_map_rgb(220, 10, 10);
    const char* titulo = "VOCE MORREU";

    if (venceu == true) {
        cor_titulo = al_map_rgb(0, 255, 0);
        titulo = "VOCE VENCEU O JOGO!";
    }

    al_draw_filled_rectangle(centro_x - 200, 100, centro_x + 200, 230, al_map_rgb(0, 0, 0));
    al_draw_rectangle(centro_x - 200, 100, centro_x + 200, 230, cor_titulo, 2);

    al_draw_text(fonte_titulo, cor_titulo, centro_x, 112, ALLEGRO_ALIGN_CENTER, titulo);
    al_draw_textf(fonte_texto, al_map_rgb(255, 255, 255), centro_x, 152, ALLEGRO_ALIGN_CENTER, "Jogador: %s", nome);
    al_draw_textf(fonte_texto, al_map_rgb(255, 235, 120), centro_x, 176, ALLEGRO_ALIGN_CENTER, "Score final: %d", score);
    al_draw_text(fonte_texto, al_map_rgb(220, 220, 220), centro_x, 202, ALLEGRO_ALIGN_CENTER, "ENTER para voltar ao menu");
}

// Tela para digitar o apelido de 3 letras
void desenhar_tela_nome(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_letra, ALLEGRO_FONT* fonte_texto, char nome[], int tamanho) {
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cinza = al_map_rgb(90, 90, 90);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 235, 120);

    float centro_x = width / 2.0f;
    float topo_caixas = height * 0.4f;

    al_draw_text(fonte_titulo, branco, centro_x, height * 0.2f, ALLEGRO_ALIGN_CENTER, "DIGITE SEU APELIDO");
    al_draw_text(fonte_texto, cinza, centro_x, height * 0.2f + 40, ALLEGRO_ALIGN_CENTER, "3 letras, estilo arcade");

    for (int i = 0; i < 3; i++) {
        float x1 = centro_x - 105 + i * 75;
        float y1 = topo_caixas;
        float x2 = x1 + 60;
        float y2 = y1 + 70;

        ALLEGRO_COLOR cor_caixa = cinza;
        if (i == tamanho) {
            cor_caixa = amarelo;     // caixa onde a próxima letra vai entrar
        }

        al_draw_rectangle(x1, y1, x2, y2, cor_caixa, 2);

        if (i < tamanho) {
            al_draw_textf(fonte_letra, branco, (x1 + x2) / 2.0f, y1 + 19, ALLEGRO_ALIGN_CENTER, "%c", nome[i]);
        }
    }

    al_draw_text(fonte_texto, branco, centro_x, height * 0.7f, ALLEGRO_ALIGN_CENTER, "ENTER: confirmar | BACKSPACE: apagar | ESC: voltar");
}

// Tela do ranking (Top 10)
void desenhar_ranking(int width, int height, ALLEGRO_FONT* fonte_titulo, ALLEGRO_FONT* fonte_texto, RegistroRanking ranking[], int total) {
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cinza = al_map_rgb(90, 90, 90);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 235, 120);

    float centro_x = width / 2.0f;

    al_draw_text(fonte_titulo, branco, centro_x, 10, ALLEGRO_ALIGN_CENTER, "RANKING - TOP 10");
    al_draw_line(20, 46, width - 20, 46, cinza, 1);

    if (total == 0) {
        al_draw_text(fonte_texto, branco, centro_x, 200, ALLEGRO_ALIGN_CENTER, "Nenhuma partida registrada ainda.");
    }

    for (int i = 0; i < total; i++) {
        ALLEGRO_COLOR cor = branco;
        if (i == 0) {
            cor = amarelo;           // o primeiro lugar fica destacado
        }

        int y = 70 + i * 36;

        al_draw_textf(fonte_texto, cor, centro_x - 160, y, ALLEGRO_ALIGN_LEFT, "%d.", i + 1);
        al_draw_textf(fonte_texto, cor, centro_x - 90, y, ALLEGRO_ALIGN_LEFT, "%s", ranking[i].nome);
        al_draw_textf(fonte_texto, cor, centro_x + 160, y, ALLEGRO_ALIGN_RIGHT, "%d", ranking[i].score);
    }

    al_draw_text(fonte_texto, amarelo, centro_x, height - 30, ALLEGRO_ALIGN_CENTER, "Pressione ESC ou ENTER para voltar");
}

// Score final = score + 10 por RP que sobrou + 200 por ponto de vida
int calcular_score_final(Player* player, SistemaQuimico* sq) {
    int bonus_rp = sq->research_point * 10;
    int bonus_vida = player->vida * 200;

    return player->score + bonus_rp + bonus_vida;
}

// Adiciona uma linha "NOME SCORE" no final do ranking.txt
void salvar_score(const char* nome, int score) {
    FILE* arquivo = fopen("ranking.txt", "a");

    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "%s %d\n", nome, score);
    fclose(arquivo);
}

// Lê o ranking.txt, guarda os 10 maiores scores e ordena (Bubble Sort)
int carregar_ranking(RegistroRanking ranking[]) {
    int total = 0;
    char nome[4] = { 0 };
    int score = 0;

    FILE* arquivo = fopen("ranking.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fscanf(arquivo, "%3s %d", nome, &score) == 2) {
        RegistroRanking novo = { { 0 }, 0 };

        for (int k = 0; k < 4; k++) {
            novo.nome[k] = nome[k];
        }
        novo.score = score;

        if (total < TOP_RANKING) {
            ranking[total] = novo;
            total++;
        }
        else {
            // lista cheia: troca o menor score se o novo for maior
            int menor = 0;
            for (int i = 1; i < TOP_RANKING; i++) {
                if (ranking[i].score < ranking[menor].score) {
                    menor = i;
                }
            }

            if (novo.score > ranking[menor].score) {
                ranking[menor] = novo;
            }
        }
    }

    fclose(arquivo);

    // Bubble Sort: maior score primeiro
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (ranking[j].score < ranking[j + 1].score) {
                RegistroRanking aux = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = aux;
            }
        }
    }

    return total;
}