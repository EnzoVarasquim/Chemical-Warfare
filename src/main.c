#include <stdio.h>
#include <time.h>
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


//salve
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
    int width = 640;
    int height = 480;

    bool rodando = true;
    bool saindo = false;

    Player player = init_player();

    // Aloca memória pra guardar 5 inimigos (dps tem q mudar pra quantidade aumentar por wave)
    int total_inimigos = 5;
    Inimigo* qtd_inimigos = (Inimigo*)al_malloc(total_inimigos * sizeof(Inimigo));

    // Inicializa cada inimigo em uma posição diferente
    for (int i = 0; i < total_inimigos; i++) {
        int tamanho_inimigo = 20;
        int posX = tamanho_inimigo + (rand() % (width - 2 * tamanho_inimigo + 1));
        int posY = tamanho_inimigo + (rand() % (height - 2 * tamanho_inimigo + 1));
        qtd_inimigos[i] = init_inimigo(posX, posY);
    }

    Mouse mouse = init_mouse();
    Keyboard keyboard = init_keyboard();
    Tiro num_tiros[MAX_TIROS];

    //percorre o array dos tiros pra iniciar todos igualmente
    for (int i = 0; i < MAX_TIROS; i++) {
        num_tiros[i] = init_tiro();
    }

    ALLEGRO_DISPLAY* display = al_create_display(width, height);
    al_set_window_title(display, "Chemical Warfare");

    ALLEGRO_FONT* fonte_grande = al_load_font("assets/fonts/font.ttf", 24, 0);
    ALLEGRO_FONT* fonte_media = al_load_font("assets/fonts/font.ttf", 18, 0);
    ALLEGRO_FONT* fonte_pequena = al_load_font("assets/fonts/font.ttf", 12, 0);

    //som do tiro
    printf("Diretorio de trabalho: %s\n", al_get_current_directory());

    ALLEGRO_SAMPLE* som_tiro =
        al_load_sample("assets/sounds/som_tiro_lase.ogg");


    if (!al_reserve_samples(16)) {
        printf("ERRO: nao conseguiu reservar os canais de audio!\n");
        return 1;
    }

    if (!som_tiro) {
        printf("ERRO: nao conseguiu carregar o som!\n");
        return 1;
    }


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); //fps
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

    while (rodando) {
        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue, &event);
        al_hide_mouse_cursor(display);

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
                mouse.left_button = true;
                disparar_tiro(&mouse, &player, num_tiros);
                //
                al_play_sample(som_tiro,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

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
                reset(&player, qtd_inimigos, total_inimigos, num_tiros);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                al_rest(1.0);
                saindo = true;
            }
        }

        //se soltar uma tecla
        if (event.type == ALLEGRO_EVENT_KEY_UP) {
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
        }

        //atualizando os dados em tempo real ;P
        if (event.type == ALLEGRO_EVENT_TIMER) {
            //player
            atualizar_player(&player, keyboard);
            colisao_tela(&player);

            //tiros
            atualizar_posicao_tiros(num_tiros, width, height);

            // inimigos
            for (int i = 0; i < total_inimigos; i++) {
                seguir_player(&qtd_inimigos[i], player.x, player.y);
            }

            // checar a colisão entre os inimigos
            for (int i = 0; i < total_inimigos; i++) {
                for (int j = i + 1; j < total_inimigos; j++) {
                    colisao_inimigos(&qtd_inimigos[i], &qtd_inimigos[j]);
                }
            }

            //checar a colisão do inimigo com os tiros
            for (int i = 0; i < total_inimigos; i++) {
                colisao_tiro(&qtd_inimigos[i], num_tiros);
            }
        }

        //quando acaba todos os eventos de cima, isso aq desenha os elementos
        if (al_is_event_queue_empty(event_queue)) {
            //background bonitin q eu fiz :)
            desenhar_background();

            if (saindo) {
                al_draw_text(fonte_grande, al_map_rgb(255, 255, 255), width / 2, height / 3, ALLEGRO_ALIGN_CENTER, "Saindo...");
                al_flip_display();
                al_rest(0.5);
                rodando = false;
            }
            else {
                desenhar_tiros(num_tiros);

                for (int i = 0; i < total_inimigos; i++) {
                    desenhar_inimigo(qtd_inimigos[i]);

                    if (qtd_inimigos[i].vivo) {
                        al_draw_textf(fonte_pequena, al_map_rgb(255, 255, 255), qtd_inimigos[i].x - 3, qtd_inimigos[i].y - 15, ALLEGRO_ALIGN_LEFT, "%d HP", qtd_inimigos[i].vida);
                    }
                }

                desenhar_player(player);
                desenhar_mira(&mouse);

                //textos na tela
                al_draw_text(fonte_media, al_map_rgb(255, 255, 255), width / 2, 10, ALLEGRO_ALIGN_CENTRE, "TESTES");

                al_flip_display();
            }
        }
    }

    //libera os inimigos da memoria
    al_free(qtd_inimigos);

   // desliga os eventos e desisntala as biblioteca quando fecha a tela
    al_destroy_sample(som_tiro);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(fonte_grande);
    al_destroy_font(fonte_media);
    al_destroy_font(fonte_pequena);

    al_uninstall_mouse();
    al_uninstall_keyboard();
    al_uninstall_audio();

    al_shutdown_primitives_addon();
    al_uninstall_system();

    return 0;
}
