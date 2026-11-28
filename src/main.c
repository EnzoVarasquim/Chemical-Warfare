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

Tiro tiros_atirador[MAX_TIROS];

int main() 
{
    srand(time(NULL));

    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    int width = 960;
    int height = 540;

    bool rodando = true;
    bool saindo = false;
    bool redesenhar = false;
    bool jogo_iniciado = false;
    bool em_tutorial = false;
    bool em_ranking = false;
    bool em_nome = false;
    bool fim_de_jogo = false;
    bool score_salvo = false;
    int score_final = 0;

    char nome_jogador[4] = { 0 };
    int tamanho_nome = 0;

    RegistroRanking ranking[TOP_RANKING] = { 0 };
    int total_ranking = 0;

    Player player = init_player(width, height);

    SistemaQuimico sq;
    inicializar_sistema_quimico(&sq);

    double tempo_mensagem_bloqueada = 0.0;

    Wave wave;
    init_wave(&wave);

    // Sistema de ataques do boss
    AtaqueBoss ataque_boss;
    init_ataque_boss(&ataque_boss);

    ItemLab itens_lab[3] = { 0 };
    init_itens_lab(itens_lab);

    int total_inimigos = 3; // Mantém a contagem original de inimigos por wave
    Inimigo* qtd_inimigos = (Inimigo*)malloc(total_inimigos * sizeof(Inimigo));

    Mouse mouse = init_mouse();
    Keyboard keyboard = init_keyboard();
    Tiro num_tiros[MAX_TIROS] = { 0 };

    for (int i = 0; i < MAX_TIROS; i++) {
        num_tiros[i] = init_tiro();
    }

    // Usa a mesma configuração de tela da versão sem menu.
    // Mantém a área lógica do jogo em 960x540 para preservar
    // as posições do player, inimigos e interface.
    al_set_new_display_flags(ALLEGRO_MAXIMIZED | ALLEGRO_NOFRAME);
    ALLEGRO_DISPLAY* display = al_create_display(width, height);

    if (!display) {
        printf("Erro ao criar a janela do jogo!\n");
        free(qtd_inimigos);
        return 1;
    }

    al_set_window_title(display, "Chemical Warfare");

    // Escala exclusiva do MENU. O jogo continua usando sua área lógica 960x540.
    int largura_display = al_get_display_width(display);
    int altura_display = al_get_display_height(display);

    // Mantém a proporção do menu e centraliza em qualquer resolução.
    float escala_menu = fminf(
        (float)largura_display / (float)width,
        (float)altura_display / (float)height
    );

    float margem_menu_x = (largura_display - (width * escala_menu)) / 2.0f;
    float margem_menu_y = (altura_display - (height * escala_menu)) / 2.0f;

    // Conversão do mouse usada pelo jogo (não alterar a lógica do gameplay).
    float escala_mouse_x = (float)width / (float)largura_display;
    float escala_mouse_y = (float)height / (float)altura_display;

    // Opção selecionada pelo teclado no menu: 0 = JOGAR, 1 = TUTORIAL, 2 = SAIR.
    int opcao_menu = 0;

    // Reposiciona o jogador no centro da área lógica 960x540.
    player = init_player(width, height);

    ALLEGRO_FONT* fonte_grande = al_load_font("assets/fonts/font.ttf", 24, 0);
    ALLEGRO_FONT* fonte_media_1 = al_load_font("assets/fonts/font.ttf", 18, 0);
    ALLEGRO_FONT* fonte_media_2 = al_load_font("assets/fonts/font.ttf", 16, 0);
    ALLEGRO_FONT* fonte_pequena = al_load_font("assets/fonts/font.ttf", 12, 0);

    //fontes do menu
    ALLEGRO_FONT* fonte_menu_titulo = al_load_font("assets/fonts/font.ttf", 56, 0);
    ALLEGRO_FONT* fonte_menu_botao = al_load_font("assets/fonts/font.ttf", 28, 0);

    // Posições do menu ajustadas para a área lógica de 960x540.
    int centro_menu_x = width / 2;
    int largura_botao = 216;
    int altura_botao = 54;
    int altura_titulo = al_get_font_line_height(fonte_menu_titulo);

    // Botão JOGAR
    int menu_jogar_x1 = centro_menu_x - largura_botao / 2;
    int menu_jogar_x2 = centro_menu_x + largura_botao / 2;
    int menu_jogar_y1 = (int)(height * 0.42f);

    //debugs
    printf("Diretorio de trabalho: %s\n", al_get_current_directory());
    if (!al_install_audio()) return 1;
    if (!al_init_acodec_addon()) return 1;
    if (!al_reserve_samples(16)) return 1;

    ALLEGRO_SAMPLE* som_tiro = al_load_sample("assets/sounds/som_tiro_lase.ogg");

    //musica do menu inicial
    ALLEGRO_AUDIO_STREAM* musica_menu = al_load_audio_stream("assets/sounds/musica-menu.ogg", 4, 2048);

    //musica principal do jogo
    ALLEGRO_AUDIO_STREAM* musica_background = al_load_audio_stream("assets/sounds/musica_background.ogg", 4, 2048);

    if (!som_tiro || !musica_background || !musica_menu) return 1;

    //anexa as duas musicas ao mixer
    al_attach_audio_stream_to_mixer(musica_menu, al_get_default_mixer());
    al_attach_audio_stream_to_mixer(musica_background, al_get_default_mixer());

    //configura as duas musicas para repetir
    al_set_audio_stream_playmode(musica_menu, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_playmode(musica_background, ALLEGRO_PLAYMODE_LOOP);

    //inicia somente a musica do menu
    al_set_audio_stream_playing(musica_menu, true);
    al_set_audio_stream_playing(musica_background, false);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_start_timer(timer);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_display(display);
        return 1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Mantém o cursor do mouse oculto desde o menu inicial.
    // A navegação do menu é feita somente pelo teclado.
    al_hide_mouse_cursor(display);

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            printf("SAIU: DISPLAY_CLOSE\n");
            rodando = false;
        }

        // Se a janela mudar de tamanho, recalcula a escala do menu.
        if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);

            largura_display = al_get_display_width(display);
            altura_display = al_get_display_height(display);

            escala_menu = fminf((float)largura_display / (float)width, (float)altura_display / (float)height);

            margem_menu_x = (largura_display - (width * escala_menu)) / 2.0f;
            margem_menu_y = (altura_display - (height * escala_menu)) / 2.0f;

            escala_mouse_x = (float)width / (float)largura_display;
            escala_mouse_y = (float)height / (float)altura_display;
        }

        //atribui a posição do mouse pra variavel
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mouse.x = (int)(event.mouse.x * escala_mouse_x);
            mouse.y = (int)(event.mouse.y * escala_mouse_y);

        }

        // Clique do mouse: utilizado somente para disparar durante o jogo.
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button == 1) {
                mouse.left_button = true;

                // Disparo durante o jogo.
                if (jogo_iniciado && player.vivo) {
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

        // Tela de tutorial: ESC ou ENTER volta ao menu.
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && !jogo_iniciado && em_tutorial) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE ||
                event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                em_tutorial = false;
            }
        }
        // Tela de ranking: ESC ou ENTER volta ao menu.
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && !jogo_iniciado && em_ranking) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE ||
                event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                em_ranking = false;
            }
        }
        // Tela do nome: aqui o KEY_DOWN só trata o ESC (as letras vêm no KEY_CHAR).
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && !jogo_iniciado && em_nome) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                em_nome = false;
            }
        }
        // Navegação do menu inicial pelo teclado.
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && !jogo_iniciado) {
            if (event.keyboard.keycode == ALLEGRO_KEY_W ||
                event.keyboard.keycode == ALLEGRO_KEY_UP) {
                opcao_menu--;

                if (opcao_menu < 0) {
                    opcao_menu = 3;
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_S ||
                event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                opcao_menu++;

                if (opcao_menu > 3) {
                    opcao_menu = 0;
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                // JOGAR: vai para a tela de digitar o nome
                if (opcao_menu == 0) {
                    em_nome = true;
                    tamanho_nome = 0;
                    nome_jogador[0] = '\0';
                }
                // TUTORIAL
                else if (opcao_menu == 1) {
                    em_tutorial = true;
                }
                // RANKING
                else if (opcao_menu == 2) {
                    total_ranking = carregar_ranking(ranking);
                    em_ranking = true;
                }
                // SAIR
                else if (opcao_menu == 3) {
                    printf("SAIU: opcao SAIR (opcao_menu=%d)\n", opcao_menu);   //debug
                    rodando = false;
                }
            }
        }

        // Digitação do apelido (3 letras)
        if (event.type == ALLEGRO_EVENT_KEY_CHAR && !jogo_iniciado && em_nome) {
            int tecla = event.keyboard.keycode;
            int letra = event.keyboard.unichar;

            if (tecla == ALLEGRO_KEY_BACKSPACE) {
                if (tamanho_nome > 0) {
                    tamanho_nome--;
                    nome_jogador[tamanho_nome] = '\0';
                }
            }
            else if (tecla == ALLEGRO_KEY_ENTER) {
                // só confirma com exatamente 3 letras
                if (tamanho_nome == 3) {

                    // ---- INÍCIO DA PARTIDA (corpo que estava no JOGAR) ----
                    int novo_total = 3;
                    Inimigo* temp = (Inimigo*)realloc(qtd_inimigos, novo_total * sizeof(Inimigo));
                    if (temp != NULL) {
                        qtd_inimigos = temp;
                        total_inimigos = novo_total;
                    }

                    inicializar_sistema_quimico(&sq);
                    init_wave(&wave);
                    init_ataque_boss(&ataque_boss);
                    init_itens_lab(itens_lab);
                    keyboard = init_keyboard();
                    tempo_mensagem_bloqueada = 0.0;

                    // player (score = 0, combo = 1), inimigos e tiros do player
                    reset(&player, qtd_inimigos, total_inimigos, num_tiros, &wave, width, height);
                    for (int i = 0; i < MAX_TIROS; i++) {
                        tiros_atirador[i] = init_tiro();
                    }

                    mouse.left_button = false;
                    mouse.x = player.x;
                    mouse.y = player.y;

                    fim_de_jogo = false;
                    score_salvo = false;
                    score_final = 0;

                    em_nome = false;
                    jogo_iniciado = true;

                    // Para a música do menu e inicia a música do jogo.
                    al_set_audio_stream_playing(musica_menu, false);
                    al_set_audio_stream_playing(musica_background, true);

                    al_hide_mouse_cursor(display);
                }
            }
            else if (tamanho_nome < 3) {
                // aceita só letras; minúscula vira maiúscula
                if (letra >= 'a' && letra <= 'z') {
                    letra = letra - 32;
                }

                if (letra >= 'A' && letra <= 'Z') {
                    nome_jogador[tamanho_nome] = (char)letra;
                    tamanho_nome++;
                    nome_jogador[tamanho_nome] = '\0';
                }
            }
        }

        //se pressionar uma tecla
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && jogo_iniciado) {
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

            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                // Fim de jogo: volta ao menu principal
                if (fim_de_jogo == true) {
                    jogo_iniciado = false;
                    fim_de_jogo = false;
                    score_salvo = false;
                    opcao_menu = 0;
                    keyboard = init_keyboard();
                    mouse.left_button = false;

                    al_set_audio_stream_playing(musica_background, false);
                    al_set_audio_stream_playing(musica_menu, true);
                }
                // Saída do Laboratório
                else if (wave.laboratorio && wave.andamento) {
                    wave.laboratorio = false;
                    wave.combates_vencidos = 0;
                    wave.finalizado = true;
                    wave.andamento = false;

                    int novo_total = wave.atual + 2;
                    Inimigo* temp = (Inimigo*)realloc(qtd_inimigos, novo_total * sizeof(Inimigo));
                    if (temp != NULL) {
                        qtd_inimigos = temp;
                        total_inimigos = novo_total;
                    }
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                al_rest(1.0);
                saindo = true;
            }
        }

        //se soltar uma tecla
        if (event.type == ALLEGRO_EVENT_KEY_UP && jogo_iniciado) {

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
                if (sq.mensagem_arma_bloqueada) tempo_mensagem_bloqueada = al_get_time();
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, ARMA_ACIDA);
                if (sq.mensagem_arma_bloqueada) tempo_mensagem_bloqueada = al_get_time();
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_3) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, ARMA_BASICA);
                if (sq.mensagem_arma_bloqueada) tempo_mensagem_bloqueada = al_get_time();
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_4) {
                sq.mensagem_arma_bloqueada = false;
                selecionar_arma(&sq, ARMA_METALICA);
                if (sq.mensagem_arma_bloqueada) tempo_mensagem_bloqueada = al_get_time();
            }
        }

        //atualizando os dados em tempo real
        if (event.type == ALLEGRO_EVENT_TIMER && jogo_iniciado) {

            // Esconde a mensagem de arma bloqueada depois de 3 segundos
            if (sq.mensagem_arma_bloqueada && al_get_time() - tempo_mensagem_bloqueada >= 3.0) {
                sq.mensagem_arma_bloqueada = false;
            }

            if (!wave.vitoria) {
                atualizar_player(&player, keyboard);
                colisao_tela(&player, width, height);

                atualizar_posicao_tiros(num_tiros, width, height);
                atualizar_posicao_tiros(tiros_atirador, width, height);

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

                    if (qtd_inimigos[i].tipo == TIPO_BOSS) {
                        // fase 1: persegue; fase 2: o boss.c o centraliza e ele fica parado
                        if (qtd_inimigos[i].vida > VIDA_FASE_2) {
                            qtd_inimigos[i].vel = 4;
                            seguir_player(&qtd_inimigos[i], &player);
                        }
                    }
                    else if (qtd_inimigos[i].tipo == TIPO_ATIRADOR) {
                        disparar_tiro_atirador(&qtd_inimigos[i], &player, tiros_atirador);
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

                for (int i = 0; i < total_inimigos; i++) {
                    colisao_player(&qtd_inimigos[i], &player);
                }

                //atualizar a moeda
                atualizar_moedas(qtd_inimigos, total_inimigos, &player, &sq);

                if (wave.laboratorio) {
                    computar_compra_item(&sq, &player, itens_lab, width, height);
                }

                for (int i = 0; i < total_inimigos; i++) {
                    colisao_tiro(&qtd_inimigos[i], num_tiros, &sq, &player);
                }

                colisao_tiro_atirador(tiros_atirador, &player);
            }

            // Detecta o fim da partida e salva o score UMA vez só
            fim_de_jogo = (wave.vitoria == true || player.vivo == false);

            if (fim_de_jogo == true && score_salvo == false) {
                score_final = calcular_score_final(&player, &sq);
                salvar_score(nome_jogador, score_final);
                score_salvo = true;
            }
        }

        //quando acaba todos os eventos de cima, isso desenha os elementos
        if (al_is_event_queue_empty(event_queue) && redesenhar == true) {
            redesenhar = false;

            //TELAS DO MENU (menu, tutorial, ranking e nome)
            if (!jogo_iniciado) {
                ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
                ALLEGRO_COLOR fundo_botao = al_map_rgb(35, 35, 35);

                // A transformação afeta somente o desenho do menu.
                ALLEGRO_TRANSFORM transformacao_menu;
                al_identity_transform(&transformacao_menu);
                al_translate_transform(&transformacao_menu, margem_menu_x, margem_menu_y);
                al_scale_transform(&transformacao_menu, escala_menu, escala_menu);
                al_use_transform(&transformacao_menu);

                al_clear_to_color(al_map_rgb(0, 0, 0));

                if (em_tutorial == true) {
                    desenhar_tutorial(width, height, fonte_grande, fonte_media_2, fonte_pequena);
                }
                else if (em_ranking == true) {
                    desenhar_ranking(width, height, fonte_grande, fonte_media_1, ranking, total_ranking);
                }
                else if (em_nome == true) {
                    desenhar_tela_nome(width, height, fonte_grande, fonte_menu_botao, fonte_media_2, nome_jogador, tamanho_nome);
                }
                else {
                    //título
                    al_draw_text(fonte_menu_titulo, branco, centro_menu_x, height * 0.12f, ALLEGRO_ALIGN_CENTER, "CHEMICAL");
                    al_draw_text(fonte_menu_titulo, branco, centro_menu_x, height * 0.12f + altura_titulo, ALLEGRO_ALIGN_CENTER, "WARFARE");

                    //botões
                    const char* textos_menu[4] = { "JOGAR", "TUTORIAL", "RANKING", "SAIR" };

                    for (int i = 0; i < 4; i++) {
                        int y1 = menu_jogar_y1 + i * (altura_botao + 14);
                        int y2 = y1 + altura_botao;

                        // destaque da opção selecionada
                        if (opcao_menu == i) {
                            al_draw_filled_rectangle(menu_jogar_x1, y1, menu_jogar_x2, y2, fundo_botao);
                        }

                        al_draw_rectangle(menu_jogar_x1, y1, menu_jogar_x2, y2, branco, 2.0f);
                        al_draw_text(fonte_menu_botao, branco, centro_menu_x, y1 + (altura_botao - al_get_font_line_height(fonte_menu_botao)) / 2.0f, ALLEGRO_ALIGN_CENTER, textos_menu[i]);
                    }
                }

                al_flip_display();
            }
            //JOGO
            else {
                // Garante que o gameplay continue com a transformação original.
                ALLEGRO_TRANSFORM transformacao_jogo;
                al_identity_transform(&transformacao_jogo);
                al_use_transform(&transformacao_jogo);

                //UI do jogo
                desenhar_background(width, height);
                desenhar_barra_lateral(width, height);

                if (saindo) {
                    printf("SAIU: saindo=true\n");   //debug
                    al_draw_text(fonte_grande, al_map_rgb(255, 255, 255), width / 1.6, height / 2.25, ALLEGRO_ALIGN_CENTER, "Saindo...");
                    al_flip_display();
                    al_rest(0.5);
                    rodando = false;
                }
                else {
                    desenhar_tiros(num_tiros);
                    desenhar_tiros(tiros_atirador);

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

                    desenhar_moedas_mortos(qtd_inimigos, total_inimigos, fonte_pequena);

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
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 84, ALLEGRO_ALIGN_LEFT, "Research points: %d", sq.research_point);
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 235, 120), 25, 99, ALLEGRO_ALIGN_LEFT, "Score: %d  Combo: x%d", player.score, player.combo);

                    // UI de Elementos Químicos
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 125, ALLEGRO_ALIGN_LEFT, "H: %d", sq.qtd_H);
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 145, ALLEGRO_ALIGN_LEFT, "Cl: %d", sq.qtd_Cl);
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, 165, ALLEGRO_ALIGN_LEFT, "Na: %d", sq.qtd_Na);
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 120, 125, ALLEGRO_ALIGN_LEFT, "O: %d", sq.qtd_O);
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 120, 145, ALLEGRO_ALIGN_LEFT, "S: %d", sq.qtd_S);
                    al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 120, 165, ALLEGRO_ALIGN_LEFT, "C: %d", sq.qtd_C);

                    // Progresso das receitas
                    al_draw_text(fonte_media_2, al_map_rgb(255, 255, 255), 25, 196, ALLEGRO_ALIGN_LEFT, "Receitas");
                    desenhar_progresso_receitas(&sq, fonte_pequena, 25, 218);

                    // UI das reações do laboratório
                    al_draw_text(fonte_media_2, al_map_rgb(255, 255, 255), 25, 338, ALLEGRO_ALIGN_LEFT, "Reações adquiridas");

                    int linha_atual = 0;
                    for (int i = 0; i < 3; i++) {
                        if (itens_lab[i].contador_compra > 0) {
                            int posY = 362 + (linha_atual * 20);

                            al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), 25, posY, ALLEGRO_ALIGN_LEFT, "* %s (x%d)", itens_lab[i].nome, itens_lab[i].contador_compra);
                            linha_atual++;
                        }
                    }

                    // Pop-up de arma desbloqueada (3 segundos, sem pausar o jogo)
                    if (sq.arma_desbloqueada != TIRO_PADRAO &&
                        al_get_time() - sq.tempo_mensagem_desbloqueio < 3.0) {

                        const char* titulo_popup = "";
                        const char* dica_popup = "";
                        ALLEGRO_COLOR cor_popup = al_map_rgb(255, 255, 255);

                        switch (sq.arma_desbloqueada) {
                        case ARMA_ACIDA:
                            titulo_popup = "Arma Acida Desbloqueada - HCl";
                            dica_popup = "Pressione 2 para equipar";
                            cor_popup = al_map_rgb(80, 255, 80);
                            break;
                        case ARMA_BASICA:
                            titulo_popup = "Arma Basica Desbloqueada - NaOH";
                            dica_popup = "Pressione 3 para equipar";
                            cor_popup = al_map_rgb(80, 180, 255);
                            break;
                        case ARMA_METALICA:
                            titulo_popup = "Arma Metalica Desbloqueada - NaCl";
                            dica_popup = "Pressione 4 para equipar";
                            cor_popup = al_map_rgb(180, 70, 255);
                            break;
                        default:
                            break;
                        }

                        desenhar_popup(width, fonte_media_2, fonte_pequena, titulo_popup, dica_popup, cor_popup);
                    }

                    // Pop-up de arma bloqueada (mesmo estilo, borda vermelha)
                    if (sq.mensagem_arma_bloqueada) {
                        desenhar_popup(width, fonte_media_2, fonte_pequena, "Arma Bloqueada", "Complete a reacao quimica para desbloquear", al_map_rgb(255, 50, 50));
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

                    if (wave.laboratorio == true) {
                        al_draw_text(fonte_grande, al_map_rgb(0, 120, 255), width / 1.6, 60, ALLEGRO_ALIGN_CENTER, "LABORATÓRIO");
                        al_draw_text(fonte_media_1, al_map_rgb(255, 255, 255), width / 1.6, 90, ALLEGRO_ALIGN_CENTER, "Compre reações com research points");

                        desenhar_itens_lab(&sq, itens_lab, fonte_pequena);

                        al_draw_filled_rectangle(width - 505, height - 20, width - 215, height - 40, al_map_rgb(0, 0, 0));
                        al_draw_rectangle(width - 505, height - 20, width - 215, height - 40, al_map_rgb(50, 50, 50), 1);
                        al_draw_text(fonte_pequena, al_map_rgb(255, 255, 255), width / 1.6, height - 37, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para sair do Laboratório");
                    }

                    if (wave.em_pausa == true) {
                        double restante = TEMPO_PAUSA_WAVE - (al_get_time() - wave.tempo_inicio_pausa);
                        int segundos = (int)restante;
                        if (restante > segundos) {
                            segundos++;
                        }

                        al_draw_text(fonte_grande, al_map_rgb(255, 235, 120), width / 1.6, 60, ALLEGRO_ALIGN_CENTER, "Recolha os elementos!");
                        al_draw_textf(fonte_media_1, al_map_rgb(255, 255, 255), width / 1.6, 90, ALLEGRO_ALIGN_CENTER, "Proxima wave em %d...", segundos);
                    }

                    if (fim_de_jogo == true) {
                        desenhar_fim_de_jogo(width, fonte_grande, fonte_media_1, wave.vitoria, nome_jogador, score_final);
                    }

                    al_flip_display();
                }
            }
        }
    }

    free(qtd_inimigos);

    // desliga os eventos e desinstala as bibliotecas quando fecha a tela
    al_destroy_sample(som_tiro);
    al_destroy_audio_stream(musica_menu);
    al_destroy_audio_stream(musica_background);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(fonte_grande);
    al_destroy_font(fonte_media_1);
    al_destroy_font(fonte_media_2);
    al_destroy_font(fonte_pequena);
    al_destroy_font(fonte_menu_titulo);
    al_destroy_font(fonte_menu_botao);

    al_uninstall_mouse();
    al_uninstall_keyboard();
    al_uninstall_audio();
    al_shutdown_primitives_addon();
    al_uninstall_system();

    return 0;
    
}