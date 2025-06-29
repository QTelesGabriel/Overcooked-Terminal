#ifndef CONFIG_H
#define CONFIG_H

#include <ctype.h>

#include "./mural_de_pedidos.h"
#include "./chefe_da_cozinha.h"
#include "./tripulantes.h"

extern ListaDePedidos *lista;
extern pthread_mutex_t lista_lock;
extern pthread_mutex_t ncurses_lock;

extern int proximo_id;

extern int jogo_esta_valendo;
extern pthread_mutex_t lock_jogo;

extern int quantidade_tripulantes;
extern int intervalo_pedidos;

extern int tempo_de_jogo;
extern pthread_mutex_t lock_tempo;

extern int pontuacao;

extern ChefeDaCozinha chefe;

extern Tripulante *tripulante;

typedef struct BancadaPreparo {
    int x;
    int y;
    int id;
    int ocupada;
} BancadaPreparo;

extern BancadaPreparo *bancada_preparo;

extern pthread_mutex_t preparo_lock;

typedef struct BancadaCozinha {
    int x;
    int y;
    int id;
    int ocupada;
} BancadaCozinha;

extern BancadaCozinha *bancada_cozinha;

extern pthread_mutex_t cozinha_lock;

extern int id_pedido_que_sera_feito;
extern pthread_mutex_t id_pedido_lock;

extern Comida pratos[];
extern int total_pratos;

void definir_variaveis_inicio();

#endif