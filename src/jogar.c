#include "../include/jogar.h"
#include "../include/mural_de_pedidos.h"
#include "../include/exibir_informacoes.h"
#include "../include/tripulantes.h"
#include "../include/chefe_da_cozinha.h"
#include "../include/config.h"
#include "../include/utils.h"

#include <ncurses.h>

void* thread_mural_de_pedidos(void* arg) {
    mural_de_pedidos(intervalo_pedidos);
    return NULL;
}

void* thread_exibir_informacoes(void* arg) {
    exibir_informacoes();
    return NULL;
}

void* thread_tripulantes(void* arg) {
    Tripulante *t = (Tripulante *) arg;
    tripulantes(t);
    return NULL;
}

void* thread_chefe_da_cozinha(void* arg) {
    chefe_da_cozinha();
    return NULL;
}

void* thread_temporizador(void* arg) {
    while (tempo_de_jogo > 0 && jogo_esta_valendo) {
        sleep(1);
        pthread_mutex_lock(&lock_tempo);
        tempo_de_jogo--;
        pthread_mutex_unlock(&lock_tempo);
    }

    pthread_mutex_lock(&lock_jogo);
    jogo_esta_valendo = 0;
    pthread_mutex_unlock(&lock_jogo);

    for (int i = 0; i < quantidade_tripulantes; i++) {
        pthread_mutex_lock(&tripulante[i].lock);
        pthread_cond_signal(&tripulante[i].cond);
        pthread_mutex_unlock(&tripulante[i].lock);
    }

    return NULL;
}

void inicializar_tripulantes() {
    for (int i = 0; i < quantidade_tripulantes; i++) {
        tripulante[i].id = i;
        tripulante[i].x = 75;
        tripulante[i].y = 11 + i * 5;
        tripulante[i].destino_x = 0;
        tripulante[i].destino_y = 0;
        tripulante[i].pedido_id = -1;
        tripulante[i].tem_pedido = 0;
        pthread_mutex_init(&tripulante[i].lock, NULL);
        pthread_cond_init(&tripulante[i].cond, NULL);
    }
}

void inicializar_bancada_preparo() {
    for (int i = 0; i < quantidade_tripulantes; i++) {
        bancada_preparo[i].id = i;
        bancada_preparo[i].x = 20 + i * 12;
        bancada_preparo[i].y = 4;
        bancada_preparo[i].ocupada = 0;
    }
}

void inicializar_bancada_cozinha() {
    for (int i = 0; i < quantidade_tripulantes; i++) {
        bancada_cozinha[i].id = i;
        bancada_cozinha[i].x = 7;
        bancada_cozinha[i].y = 10 + i * 8;
        bancada_cozinha[i].ocupada = 0;
    }
}

void jogar() {

    jogo_esta_valendo = 1;

    nodelay(stdscr, TRUE);

    pthread_t mural_de_ped;
    pthread_t exib_info;
    pthread_t trip[quantidade_tripulantes];
    pthread_t chefe_da_coz;
    pthread_t temporizador;

    // Alocando memória para a quantidade correta de tripulantes
    tripulante = malloc(sizeof(Tripulante) * quantidade_tripulantes);
    if (!tripulante) {
        perror("Erro ao alocar tripulantes");
        exit(1);
    }

    // Alocando memória para a quantidade correta de bancadas de preparo
    bancada_preparo = malloc(sizeof(BancadaPreparo) * quantidade_tripulantes);
    if (!bancada_preparo) {
        perror("Erro ao alocar bancadas de preparo");
        exit(1);
    }

    // Alocando memória para a quantidade correta de bancada de cozinhas
    bancada_cozinha = malloc(sizeof(BancadaPreparo) * quantidade_tripulantes);
    if (!bancada_cozinha) {
        perror("Erro ao alocar bancadas de cozinha");
        exit(1);
    }

    pthread_mutex_lock(&lista_lock);
    lista = (ListaDePedidos *)malloc(sizeof(ListaDePedidos));
    if (!lista) {
        perror("Erro ao alocar Lista De Pedidos");
        exit(1);
    }
    inicializar_lista(lista);
    pthread_mutex_unlock(&lista_lock);

    // Inicializando Tripulantes, Bancadas de Preparo e Bancadas de Cozinha
    inicializar_tripulantes();
    inicializar_bancada_preparo();
    inicializar_bancada_cozinha();

    // Cria thread Mural de pedidos
    pthread_create(&mural_de_ped, NULL, thread_mural_de_pedidos, NULL);
    // Cria thread Exibir Informações
    pthread_create(&exib_info, NULL, thread_exibir_informacoes, NULL);
    // Cria N threads Tripulantes
    for (int i = 0; i < quantidade_tripulantes; i++) {
        pthread_create(&trip[i], NULL, thread_tripulantes, &tripulante[i]);
    }
    // Cria thread Chefe da Cozinha
    pthread_create(&chefe_da_coz, NULL, thread_chefe_da_cozinha, NULL);
    // Cria thread Temporizador
    pthread_create(&temporizador, NULL, thread_temporizador, NULL);

    pthread_join(mural_de_ped, NULL);
    pthread_join(exib_info, NULL);
    for (int i = 0; i < quantidade_tripulantes; i++) {
        pthread_join(trip[i], NULL);
    }
    pthread_join(chefe_da_coz, NULL);
    pthread_join(temporizador, NULL);

    liberar_lista(lista);
    free(lista);
    lista = NULL;
    free(tripulante);
    free(bancada_preparo);
    free(bancada_cozinha);

    nodelay(stdscr, FALSE);

    definir_variaveis_inicio();
}