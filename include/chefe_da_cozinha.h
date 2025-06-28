#ifndef CHEFE_DA_COZINHA
#define CHEFE_DA_COZINHA

#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct ChefeDaCozinha {
    int x;
    int y;
} ChefeDaCozinha;

int colide_com_tripulante(int novo_y, int novo_x);
void chefe_da_cozinha();

#endif