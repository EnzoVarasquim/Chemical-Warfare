#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include "geral.h"
#include "player.h"
#include "tiro.h"
#include "inimigos.h"

#define MAX_TIROS 30

int main(){
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    //tela
    int width = 640;
    int height = 480;

    bool rodando = true;
    bool saindo = false;
    
    Player player = init_player();
    Inimigo inimigo = init_inimigo();
    Mouse mouse = init_mouse();
    Keyboard keyboard = init_keyboard();
    Tiro tiro = init_tiro();
    Tiro num_tiros[MAX_TIROS];

    //percorre o array dos tiros pra iniciar todos igualmente
    for (int i = 0; i < MAX_TIROS; i++) {
        num_tiros[i] = init_tiro(); 
    }

    ALLEGRO_DISPLAY* display = al_create_display(width, height);
    al_set_window_title(display, "Chemical Warfare");

    ALLEGRO_FONT* fonte = al_load_font("assets/fonts/font.ttf", 18, 0);

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

    while (rodando){
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
                disparar_tiro(&mouse, &player, num_tiros);
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
                reset(&player, &inimigo, num_tiros);
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
            colisao_tiro(&inimigo, num_tiros);
        }

        //quando acaba todos os eventos de cima, isso aq desenha os elementos
        if (al_is_event_queue_empty(event_queue)) {
            //background bonitin q eu fiz :)
            desenhar_background();

            if (saindo) {
                al_draw_text(fonte, al_map_rgb(255, 255, 255), width / 2, height / 3, ALLEGRO_ALIGN_CENTER, "Saindo...");
                al_flip_display(); 
                al_rest(0.5);      
                rodando = false;   
            }
            else {
                desenhar_tiros(num_tiros);
                desenhar_inimigo(inimigo);
                desenhar_player(player);
                desenhar_mira(&mouse);

                //textos na tela
                al_draw_text(fonte, al_map_rgb(255, 255, 255), width / 2, 10, ALLEGRO_ALIGN_CENTRE, "TESTES");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "Inimigo HP: %d", inimigo.vida);
                
                al_flip_display();
            }
        }
    }

    //desliga os eventos e desisntala as biblioteca quando fecha a tela
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(fonte);
        
    al_uninstall_mouse();
    al_uninstall_keyboard();

    al_shutdown_primitives_addon();
    al_uninstall_system();

    return 0;
}
