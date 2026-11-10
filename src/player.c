#include <allegro5/allegro_primitives.h>
#include "player.h"

Player init_player(int width, int height) {
    Player player = { width / 1.625, height / 2, 20, 5, 10, true, 0.0, 1 };
    return player;
}

void desenhar_player(Player player) {
    if (!player.vivo) return;

    ALLEGRO_COLOR cor_atual = al_map_rgb(5, 5, 200);
    float tempo_atual = al_get_time();

    //pisca várias quando leva danos
    if (player.tempo_ultimo_dano > 0.0 && tempo_atual - player.tempo_ultimo_dano < 0.5) {
        if (((int)(tempo_atual * 25)) % 2 == 0) {
            cor_atual = al_map_rgb(255, 255, 255);
        }
    }

    al_draw_filled_rectangle(player.x, player.y, player.x + player.size, player.y + player.size, cor_atual);
    al_draw_rectangle(player.x, player.y, player.x + player.size, player.y + player.size, al_map_rgb(60, 6, 150), 1);

}

void atualizar_player(Player* player, Keyboard keyboard) {
    if (!player->vivo) return;

    if (keyboard.w_pressed == true) {
        player->y -= player->vel;
    }

    if (keyboard.s_pressed == true) {
        player->y += player->vel;
    }

    if (keyboard.a_pressed == true) {
        player->x -= player->vel;
    }

    if (keyboard.d_pressed == true) {
        player->x += player->vel;
    }
}

void colisao_tela(Player* player, int width, int height) {
    if (player->x < width / 4) {                    //esquerda
        player->x = width / 4;
    }

    if (player->x > (width - player->size)) {       //direita
        player->x = (width - player->size);
    }

    if (player->y < 0) {                            //cima
        player->y = 0;
    }

    if (player->y > (height - player->size)) {      //baixo
        player->y = (height - player->size);
    }
}
void tomar_dano(Player* player, int dano) {
    if (!player->vivo) return;

    // Agora usa o campo do player em vez de uma variável static trancada
    if (al_get_time() - player->tempo_ultimo_dano >= 0.5) {
        player->vida -= dano;
        player->tempo_ultimo_dano = al_get_time();

        if (player->vida <= 0) {
            player->vida = 0;
            player->vivo = false;
        }
    }
}

Keyboard init_keyboard(void) {
    Keyboard keyboard = { false, false, false, false };
    return keyboard;
}
