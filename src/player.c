#include <allegro5/allegro_primitives.h>
#include "player.h"


Player init_player(void) {
    Player player = { 200, 240, 10, 5, 8 };
    return player;
}

void desenhar_player(Player player) {
    al_draw_rectangle(
        player.x - player.size,
        player.y - player.size,
        player.x + player.size,
        player.y + player.size,
        al_map_rgb(255, 120, 255),
        2
    );

    al_draw_filled_rectangle(
        player.x - player.size,
        player.y - player.size,
        player.x + player.size,
        player.y + player.size,
        al_map_rgb(255, 0, 255)
    );
}

void atualizar_player(Player* player, Keyboard keyboard) {
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

void colisao_tela(Player* player) {
    if (player->x < player->hitbox) {               //esquerda
        player->x = player->hitbox;
    }

    if (player->x > (640 - player->hitbox)) {       //direita
        player->x = (640 - player->hitbox);
    }

    if (player->y < player->hitbox) {               //cima
        player->y = player->hitbox;
    }

    if (player->y > (480 - player->hitbox)) {       //baixo
        player->y = (480 - player->hitbox);
    }
}


Keyboard init_keyboard(void) {
    Keyboard keyboard = { false, false, false, false };
    return keyboard;
}