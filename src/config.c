#include "../include/config.h"

int jogo_esta_valendo = 0;

int quantidade_tripulantes = 0;

int intervalo_pedidos = 0;

ChefeDaCozinha chefe = {30, 25};

ListaDePedidos *lista = NULL;
pthread_mutex_t lista_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t ncurses_lock = PTHREAD_MUTEX_INITIALIZER;

Tripulante *tripulante = NULL;

Comida pratos[] = {
    {"Pizza", 4, 6},
    {"Hamburguer", 3, 5},
    {"Sushi", 5, 4},
    {"Taco", 2, 3},
    {"Lasanha", 6, 7}
};

int total_pratos = sizeof(pratos) / sizeof(Comida);