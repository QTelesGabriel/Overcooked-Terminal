#ifndef JOGAR_H
#define JOGAR_H

#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void* thread_mural_de_pedidos(void* arg);
void* thread_exibir_informacoes(void* arg);
void* thread_tripulantes(void* arg);
void* thread_chefe_da_cozinha(void* arg);

void inicializar_tripulantes();

void jogar();

#endif