#ifndef MURAL_DE_PEDIDOS_H
#define MURAL_DE_PEDIDOS_H

#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern int jogo_esta_valendo;

typedef struct Comida{
    const char *tipo;
    int tempo_preparo;
    int tempo_cozinha;
} Comida;

typedef struct Pedido {
    Comida comida;
    int id;
    struct Pedido *proximo;
} Pedido;

typedef struct ListaDePedidos{
    int size;
    Pedido *primeiro;
} ListaDePedidos;

void mural_de_pedidos(int tempo_por_pedido);
void inicializar_lista(ListaDePedidos *lista);
Pedido* criar_pedido(Comida comida, int id);
void adicionar_pedido(ListaDePedidos *lista, Comida comida);
void remover_pedido(ListaDePedidos *lista, int id);
void liberar_lista(ListaDePedidos *lista);

#endif