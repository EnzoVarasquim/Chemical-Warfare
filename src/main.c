
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "geral.h"
#include "player.h"
#include "tiro.h"
#include "inimigos.h"
#include "reacoes.h"
#include "wave.h"
#include "boss.h"

#define MAX_TIROS 30

int main() {
    srand(time(NULL));

    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();

    //tela
    int width = 960;
    int height = 540;

    bool rodando = true;
    bool saindo = false;

    Player player = init_player(width, height);

    SistemaQuimico sq;
    inicializar_sistema_quimico(&sq);

    // Controle do tempo da mensagem de arma bloqueada
    double tempo_mensagem_bloqueada = 0.0;

    Wave wave;
    init_wave(&wave);

    // Sistema de ataques do boss
    AtaqueBoss ataque_boss;
    init_ataque_boss(&ataque_boss);

    ItemLab itens_lab[3];
    init_itens_lab(itens_lab);

    //aloca memória pra guardar os inimigos
    int total_inimigos = 3;
    Inimigo* qtd_inimigos = (Inimigo*)malloc(total_inimigos * sizeof(Inimigo));

    //inicializa cada inimigo garantindo que não nasça em cima do player
    for (int i = 0; i < total_inimigos; i++) {
        int tamanho_inimigo = 20;
        int posX, posY;
        float distancia;

        do {
            //sorteia a posição na tela
            posX = (width / 4 + tamanho_inimigo) + (rand() % (width - width / 4 - 2 * tamanho_inimigo + 1));
            posY = tamanho_inimigo + (rand() % (height - 2 * tamanho_inimigo + 1));

            //calcula a distância entre o ponto sorteado e o player
            float diffX = posX - player.x;
            float diffY = posY - player.y;
            distancia = sqrtf(diffX * diffX + diffY * diffY);

        } while (distancia < 150.0f);

        TipoInimigo tipo_aleatorio = (TipoInimigo)(rand() % 3);
        qtd_inimigos[i] = init_inimigo(posX, posY, tipo_aleatorio);
    }

    Mouse mouse = init_mouse();
    Keyboard keyboard = init_keyboard();
    Tiro num_tiros[MAX_TIROS];

    //percorre o array dos tiros pra iniciar todos igualmente
    for (int i = 0; i < MAX_TIROS; i++) {
        num_tiros[i] = init_tiro();
    }

    //cria a janela do jogo que pode ser redimensionada
    al_set_new_display_flags(ALLEGRO_MAXIMIZED | ALLEGRO_NOFRAME);
    ALLEGRO_DISPLAY* display = al_create_display(width, height);
    al_set_window_title(display, "Chemical Warfare");

    ALLEGRO_FONT* fonte_grande = al_load_font("assets/fonts/font.ttf", 24, 0);
    ALLEGRO_FONT* fonte_media_1 = al_load_font("assets/fonts/font.ttf", 18, 0);
    ALLEGRO_FONT* fonte_media_2 = al_load_font("assets/fonts/font.ttf", 16, 0);
    ALLEGRO_FONT* fonte_pequena = al_load_font("assets/fonts/font.ttf", 12, 0);

    //debugs
    printf("Diretorio de trabalho: %s\n", al_get_current_directory());

    if (!al_install_audio()) {
        printf("Erro ao inicializar audio!\n");
        return 1;
    }

    if (!al_init_acodec_addon()) {
        printf("Erro ao inicializar codecs!\n");
        return 1;
    }

    if (!al_reserve_samples(16)) {
        printf("ERRO: nao conseguiu reservar os canais de audio!\n");
        return 1;
    }

    //som do tiro
    ALLEGRO_SAMPLE* som_tiro = al_load_sample("assets/sounds/som_tiro_lase.ogg");

    //musica de fundo
    ALLEGRO_AUDIO_STREAM* musica_background = al_load_audio_stream("assets/sounds/musica_background.ogg", 4, 2048);

    if (!som_tiro) {
        printf("ERRO: nao conseguiu carregar o som!\n");
        return 1;
    }
    if (!musica_background) {
        printf("ERRO: nao conseguiu carregar a musica!\n");
        return 1;
    }

    al_attach_audio_stream_to_mixer(musica_background, al_get_default_mixer());
    al_set_audio_stream_playmode(musica_background, ALLEGRO_PLAYMODE_LOOP);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_start_timer(timer);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    //caso falhe um evento, ele fecha a tela
    if (!event_queue) {
        al_destroy_display(display);
        printf("Algum evento falhou");
        return 1;
    }

    //registra os eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_hide_mouse_cursor(display);

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        //fecha a janela
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            rodando = false;

        //atribui a posição do mouse pra variavel
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mouse.x = event.mouse.x;
            mouse.y = event.mouse.y;
        }

        //tiro
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button == 1) {
                mouse.left_button = true;

                if (player.vivo) {
                    disparar_tiro(&mouse, &player, num_tiros, sq.arma_ativa, &sq);
                    al_play_sample(som_tiro, 0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (event.mouse.button == 1) {
                mouse.left_button = false;
            }
        }

        //se pressionar uma tecla
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) {
                keyboard.w_pressed = true;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) {
                keyboard.s_pressed = true;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_A) {
                keyboard.a_pressed = true;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_D) {
                keyboard.d_pressed = true;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                init_wave(&wave);
                init_ataque_boss(&ataque_boss);
                total_inimigos = 5;
                qtd_inimigos = (Inimigo*)realloc(qtd_inimigos, total_inimigos * sizeof(Inimigo));
                reset(&player, qtd_inimigos, total_inimigos, num_tiros, width, height);
                inicializar_sistema_quimico(&sq);
                tempo_mensagem_bloqueada = 0.0;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                al_rest(1.0);
                saindo = true;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (wave.laboratorio && wave.andamento) {
                    wave.laboratorio = false;
                    wave.combates_vencidos = 0;
                    wave.finalizado = true;
                    wave.andamento = false;

                    total_inimigos = wave.atual + 2;
                }
            }
        }

        //se soltar uma tecla
        if (event.type == ALLEGRO_EVENT_KEY_UP) {

            //movimentação do player
            if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) {
                keyboard.w_pressed = false;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) {
                keyboard.s_pressed = false;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_A) {
                keyboard.a_pressed = false;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_D) {
                keyboard.d_pressed = false;
            }

            // Seleção de armas
            if (event.keyboard.keycode == ALLEGRO_KEY_1) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, TIRO_PADRAO);
                if (sq.mensagem_arma_bloqueada) {
                    tempo_mensagem_bloqueada = al_get_time();
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, ARMA_ACIDA);
                if (sq.mensagem_arma_bloqueada) {
                    tempo_mensagem_bloqueada = al_get_time();
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_3) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, ARMA_BASICA);
                if (sq.mensagem_arma_bloqueada) {
                    tempo_mensagem_bloqueada = al_get_time();
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_4) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, ARMA_METALICA);
                if (sq.mensagem_arma_bloqueada) {
                    tempo_mensagem_bloqueada = al_get_time();
                }
            }
        }

        //atualizando os dados em tempo real
        if (event.type == ALLEGRO_EVENT_TIMER) {

            // Esconde a mensagem de arma bloqueada depois de 3 segundos
            if (sq.mensagem_arma_bloqueada && al_get_time() - tempo_mensagem_bloqueada >= 3.0) {
                sq.mensagem_arma_bloqueada = false;
            }

            if (!wave.vitoria) {
                //player
                atualizar_player(&player, keyboard);
                colisao_tela(&player, width, height);

                //tiros
                atualizar_posicao_tiros(num_tiros, width, height);

                //loop das waves
                sistema_waves(&wave, &qtd_inimigos, &total_inimigos, &player, &sq, width, height);

                // Atualiza os ataques do boss (somente quando ele estiver ativo)
                for (int i = 0; i < total_inimigos; i++) {
                    if (qtd_inimigos[i].vivo && qtd_inimigos[i].tipo == TIPO_BOSS) {
                        atualizar_ataque_boss(&ataque_boss, &qtd_inimigos[i], &player, width, height);
                        colisao_ataque_boss(&ataque_boss, &qtd_inimigos[i], &player);
                    }
                }

                // Movimento dos inimigos e do chefe
                for (int i = 0; i < total_inimigos; i++) {

                    if (!qtd_inimigos[i].vivo)
                        continue;

                    // Primeira fase: chefe persegue o player rapidamente.
                    // Segunda fase: chefe permanece parado no centro.
                    if (qtd_inimigos[i].tipo == TIPO_BOSS) {
                        if (qtd_inimigos[i].vida > 75) {
                            qtd_inimigos[i].vel = 4;
                            seguir_player(&qtd_inimigos[i], &player);
                        }
                    }
                    else {
                        seguir_player(&qtd_inimigos[i], &player);
                    }
                }

                // Colisão entre os inimigos
                for (int i = 0; i < total_inimigos; i++) {

                    if (!qtd_inimigos[i].vivo)
                        continue;

                    for (int j = i + 1; j < total_inimigos; j++) {

                        if (!qtd_inimigos[j].vivo)
                            continue;

                        colisao_inimigos(&qtd_inimigos[i], &qtd_inimigos[j]);
                    }
                }

                //colisão do player com os inimigos
                for (int i = 0; i < total_inimigos; i++) {
                    colisao_player(&qtd_inimigos[i], &player);
                }

                // Reposiciona o chefe no centro somente na segunda fase.
                for (int i = 0; i < total_inimigos; i++) {

                    if (qtd_inimigos[i].vivo &&
                        qtd_inimigos[i].tipo == TIPO_BOSS &&
                        qtd_inimigos[i].vida <= 75) {

                        float limite_esquerdo = width / 4.0f;

                        qtd_inimigos[i].x = (int)(limite_esquerdo +
                            (width - limite_esquerdo - qtd_inimigos[i].size) / 2.0f);

                        qtd_inimigos[i].y = (int)(
                            (height - qtd_inimigos[i].size) / 2.0f);
                    }
                }

                //atualizar a moeda
                atualizar_moedas(qtd_inimigos, total_inimigos, &player, &sq);

                //checa se ta comprando os itens do laboratorio
                if (wave.laboratorio) {
                    computar_compra_item(&sq, &player, &itens_lab, width, height);
                }

                //checar a colisão do inimigo com os tiros
                for (int i = 0; i < total_inimigos; i++) {
                    colisao_tiro(&qtd_inimigos[i], num_tiros, &sq, &player);
                }
            }
        }

        //quando acaba todos os eventos de cima, isso desenha os elementos
        if (al_is_event_queue_empty(event_queue)) {
            //UI do jogo
            desenhar_background(width, height);
            desenhar_barra_lateral(width, height);

            if (saindo) {
                al_draw_text(fonte_grande, al_map_rgb(255, 255, 255), width / 1.6, height / 2.25, ALLEGRO_ALIGN_CENTER, "Saindo...");
                al_flip_display();
                al_rest(0.5);
                rodando = false;
            }
            else {
                desenhar_tiros(num_tiros);

                for (int i = 0; i < total_inimigos; i++) {
                    if (qtd_inimigos[i].vivo) {
                        desenhar_inimigo(qtd_inimigos[i]);
                        al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), qtd_inimigos[i].x - 3, qtd_inimigos[i].y - 15, ALLEGRO_ALIGN_LEFT, "%d HP", qtd_inimigos[i].vida);
                    }
                }

                // Desenha os projéteis e as barras giratórias do boss
                for (int i = 0; i < total_inimigos; i++) {
                    if (qtd_inimigos[i].vivo && qtd_inimigos[i].tipo == TIPO_BOSS) {
                        desenhar_ataque_boss(&ataque_boss, &qtd_inimigos[i]);
                    }
                }

                desenhar_moedas_mortos(qtd_inimigos, total_inimigos);

                desenhar_player(player);

                // Ícone da arma ativa
                ALLEGRO_COLOR cor_preview;
                switch (sq.arma_ativa) {
                case TIRO_PADRAO:
                    cor_preview = al_map_rgba(220, 220, 220, 35);
                    al_draw_filled_circle(29, 500, 15, cor_preview);
                    break;

                case ARMA_ACIDA:
                    cor_preview = al_map_rgba(80, 255, 80, 35);
                    al_draw_filled_triangle(91, 484, 74, 516, 108, 516, cor_preview);
                    break;

                case ARMA_BASICA:
                    cor_preview = al_map_rgba(80, 180, 255, 35);
                    al_draw_filled_rectangle(138, 486, 166, 514, cor_preview);
                    break;

                case ARMA_METALICA:
                    cor_preview = al_map_rgba(180, 70, 255, 35);
                    al_draw_filled_circle(207, 500, 11, cor_preview);
                    break;
                }

                // Ícone das armas desativadas
                al_draw_circle(29, 500, 15, al_map_rgba(220, 220, 220, 35), 1);
                al_draw_triangle(91, 484, 74, 516, 108, 516, al_map_rgba(220, 220, 220, 35), 1);
                al_draw_rectangle(138, 486, 166, 514, al_map_rgba(220, 220, 220, 35), 1);
                al_draw_circle(207, 500, 11, al_map_rgba(220, 220, 220, 35), 1);

                desenhar_mira(&mouse);

                // UI de Vida do Player
                al_draw_textf(fonte_media_1, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT, "HP: %d", player.vida);

                // UI de Seleção de Armas
                const char* nome_arma = "Desconhecida";

                switch (sq.arma_ativa) {
                case TIRO_PADRAO:
                    nome_arma = "Padrao";
                    break;
                case ARMA_ACIDA:
                    nome_arma = "Acida";
                    break;
                case ARMA_BASICA:
                    nome_arma = "Basica";
                    break;
                case ARMA_METALICA:
                    nome_arma = "Metalica";
                    break;
                }

                al_draw_textf(fonte_media_2, al_map_rgb(255, 255, 255), 20, 45, ALLEGRO_ALIGN_LEFT, "Arma atual: %s", nome_arma);
                al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 90, ALLEGRO_ALIGN_LEFT, "Research points: %d", sq.research_point);

                // UI de Elementos Químicos
                al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 125, ALLEGRO_ALIGN_LEFT, "H: %d", sq.qtd_H);
                al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 145, ALLEGRO_ALIGN_LEFT, "Cl: %d", sq.qtd_Cl);
                al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 165, ALLEGRO_ALIGN_LEFT, "Na: %d", sq.qtd_Na);

                // UI das reações do laboratório
                al_draw_text(fonte_media_2, al_map_rgb(255, 255, 255), 25, 200, ALLEGRO_ALIGN_LEFT, "Reações adquiridas");

                int linha_atual = 0;
                for (int i = 0; i < 3; i++) {
                    if (itens_lab[i].contador_compra > 0) {
                        int posY = 230 + (linha_atual * 20);

                        al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, posY, ALLEGRO_ALIGN_LEFT, "* %s (x%d)", itens_lab[i].nome, itens_lab[i].contador_compra);
                        linha_atual++;
                    }
                }

                //mensagem de feedback de bloqueio
                if (sq.mensagem_arma_bloqueada) {
                    al_draw_text(fonte_media_2, al_map_rgb(255, 50, 50), width / 1.6, 40, ALLEGRO_ALIGN_CENTER, "Arma não descoberta ou elementos insuficientes!");
                }

                //caixa do contador de wave
                al_draw_filled_rectangle(width - 105, 15, width - 15, 45, al_map_rgb(0, 0, 0));
                al_draw_rectangle(width - 105, 15, width - 15, 45, al_map_rgb(50, 50, 50), 1);

                //formatação do texto das waves
                int wave_exibida = wave.atual - ((wave.atual - 1) / 2);

                (wave.atual < wave.final - 3) ? al_draw_textf(fonte_media_1, al_map_rgb(255, 255, 255), width - 93, 18, ALLEGRO_ALIGN_LEFT, "WAVE %d", wave_exibida) :
                    al_draw_textf(fonte_media_1, al_map_rgb(255, 255, 255), width - 100, 18, ALLEGRO_ALIGN_LEFT, "WAVE %d", wave_exibida);

                if (wave.atual >= wave.final + 1) {
                    al_draw_filled_rectangle(width - 105, 45, width - 15, 70, al_map_rgb(0, 0, 0));
                    al_draw_rectangle(width - 105, 45, width - 15, 70, al_map_rgb(50, 50, 50), 1);
                    al_draw_filled_circle(width - 95, 57, 2, al_map_rgb(255, 50, 50));
                    al_draw_filled_circle(width - 90, 57, 3, al_map_rgb(255, 50, 50));
                    al_draw_filled_circle(width - 30, 57, 3, al_map_rgb(255, 50, 50));
                    al_draw_filled_circle(width - 25, 57, 2, al_map_rgb(255, 50, 50));
                    al_draw_text(fonte_media_1, al_map_rgb(255, 50, 50), width - 82, 45, ALLEGRO_ALIGN_LEFT, "BOSS");
                }

                if (wave.vitoria == true) {
                    al_draw_text(fonte_grande, al_map_rgb(0, 255, 0), width / 1.6, 60, ALLEGRO_ALIGN_CENTER, "VOCÊ VENCEU O JOGO!");
                    al_draw_text(fonte_media_1, al_map_rgb(255, 255, 255), width / 1.6, 90, ALLEGRO_ALIGN_CENTER, "Pressione R para reiniciar");
                }

                if (wave.laboratorio == true) {
                    al_draw_text(fonte_grande, al_map_rgb(0, 120, 255), width / 1.6, 60, ALLEGRO_ALIGN_CENTER, "LABORATÓRIO");
                    al_draw_text(fonte_media_1, al_map_rgb(255, 255, 255), width / 1.6, 90, ALLEGRO_ALIGN_CENTER, "Compre reações com research points");

                    desenhar_itens_lab(&sq, itens_lab, fonte_pequena);

                    al_draw_filled_rectangle(width - 505, height - 20, width - 215, height - 40, al_map_rgb(0, 0, 0));
                    al_draw_rectangle(width - 505, height - 20, width - 215, height - 40, al_map_rgb(50, 50, 50), 1);
                    al_draw_text(fonte_pequena, al_map_rgb(255, 255, 255), width / 1.6, height - 37, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para sair do Laboratório");
                }

                if (!player.vivo) {
                    al_draw_text(fonte_grande, al_map_rgb(220, 10, 10), width / 1.6, 60, ALLEGRO_ALIGN_CENTER, "Você morreu");
                    al_draw_text(fonte_media_1, al_map_rgb(220, 220, 220), width / 1.6, 90, ALLEGRO_ALIGN_CENTER, "Aperte R para reiniciar");
                }

                al_flip_display();
            }
        }
    }

    //libera os inimigos da memoria
    free(qtd_inimigos);

    // desliga os eventos e desinstala as bibliotecas quando fecha a tela
    al_destroy_sample(som_tiro);
    al_destroy_audio_stream(musica_background);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(fonte_grande);
    al_destroy_font(fonte_media_1);
    al_destroy_font(fonte_media_2);
    al_destroy_font(fonte_pequena);

    al_uninstall_mouse();
    al_uninstall_keyboard();
    al_uninstall_audio();

    al_shutdown_primitives_addon();
    al_uninstall_system();

    return 0;
}
