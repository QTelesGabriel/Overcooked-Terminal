#include "../include/config.h"

int jogo_esta_valendo = 0;

int proximo_id = 1;

pthread_mutex_t lock_jogo = PTHREAD_MUTEX_INITIALIZER;

int quantidade_tripulantes = 0;

int intervalo_pedidos = 0;

int tempo_de_jogo = 0;

int pontuacao = 0;

pthread_mutex_t lock_tempo = PTHREAD_MUTEX_INITIALIZER;

ChefeDaCozinha chefe = {30, 25};

ListaDePedidos *lista = NULL;
pthread_mutex_t lista_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t ncurses_lock = PTHREAD_MUTEX_INITIALIZER;

Tripulante *tripulante = NULL;

BancadaPreparo *bancada_preparo = NULL;

pthread_mutex_t preparo_lock = PTHREAD_MUTEX_INITIALIZER;

BancadaCozinha *bancada_cozinha = NULL;

pthread_mutex_t cozinha_lock = PTHREAD_MUTEX_INITIALIZER;

int id_pedido_que_sera_feito = 0;

pthread_mutex_t id_pedido_lock = PTHREAD_MUTEX_INITIALIZER;

Comida pratos[] = {
    {"Pizza", 4, 6},
    {"Hamburguer", 3, 5},
    {"Sushi", 5, 4},
    {"Taco", 2, 3},
    {"Lasanha", 6, 7}
};

int total_pratos = sizeof(pratos) / sizeof(Comida);

void definir_variaveis_inicio() {
    // Zera o estado do jogo
    jogo_esta_valendo = 0;
    pontuacao = 0;

    proximo_id = 1;

    quantidade_tripulantes = 0;
    intervalo_pedidos = 0;
    tempo_de_jogo = 0;

    // Zera ponteiros para evitar uso incorreto
    tripulante = NULL;
    bancada_preparo = NULL;
    bancada_cozinha = NULL;
    lista = NULL;

    // Reinicia ID de pedidos
    id_pedido_que_sera_feito = -1;

    // Reposiciona o chefe
    chefe.x = 30;
    chefe.y = 25;
}