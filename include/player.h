#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int size;
    int vel;
} Player;

typedef struct {
    bool w_pressed;
    bool a_pressed;
    bool s_pressed;
    bool d_pressed;
} Keyboard;

Player init_player(void);
void desenhar_player(Player player);
void atualizar_player(Player *player, Keyboard keyboard);

void colisao_tela(Player *player);

Keyboard init_keyboard(void);

#endif
