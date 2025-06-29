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
    int destino_x;
    int destino_y;
    int pedido_id;
    int tem_pedido;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Tripulante;

int colide_area_ampla(int x, int y);

int definir_destino(Tripulante *tripulante, const char *tipo);

int colide_com_tripulantes_ou_chefe(int x, int y, Tripulante *t);

void mover_tripulante(Tripulante *t);

int tempo_de_espera(int id, const char *tipo);

void tripulantes(Tripulante *tripulante);

#endif