#ifndef CONFIG_H
#define CONFIG_H

#include "./mural_de_pedidos.h"
#include "./chefe_da_cozinha.h"
#include "./tripulantes.h"

extern ListaDePedidos *lista;
extern pthread_mutex_t lista_lock;
extern pthread_mutex_t ncurses_lock;

extern int jogo_esta_valendo;
extern int quantidade_tripulantes;
extern int intervalo_pedidos;

extern ChefeDaCozinha chefe;

extern Tripulante *tripulante;

extern Comida pratos[];
extern int total_pratos;

#endif