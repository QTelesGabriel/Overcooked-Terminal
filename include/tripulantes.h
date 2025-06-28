#ifndef TRIPULANTES_H
#define TRIPULANTES_H

#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Tripulante {
    int id;
    int x;
    int y;
    int pedido_id;
    int tem_pedido;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Tripulante;

void tripulantes(Tripulante *tripulante);

#endif