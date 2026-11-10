#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int size;
    int vel;
    int vida;
    bool vivo;
    float tempo_ultimo_dano;
    int dano;
} Player;

typedef struct {
    bool w_pressed;
    bool a_pressed;
    bool s_pressed;
    bool d_pressed;
} Keyboard;

Player init_player(int width, int height);
void desenhar_player(Player player);
void atualizar_player(Player *player, Keyboard keyboard);
void tomar_dano(Player* player, int dano); 
void colisao_tela(Player *player, int width, int height);

Keyboard init_keyboard(void);

#endif
