#ifndef INIMIGOS_H
#define INIMIGOS_H

#include <stdbool.h>
#include "tiro.h"

typedef struct {
    int x;
    int y;
    int size;
    int vel;
    int vida;
    bool vivo;
} Inimigo;

Inimigo init_inimigo(void);
void desenhar_inimigo(Inimigo inimigo);
void colisao_tiro(Inimigo* inimigo, Tiro* tiros);

#endif