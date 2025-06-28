#include "../include/jogar.h"
#include "../include/mural_de_pedidos.h"
#include "../include/exibir_informacoes.h"
#include "../include/tripulantes.h"
#include "../include/chefe_da_cozinha.h"
#include "../include/config.h"

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

void inicializar_tripulantes() {
    for (int i = 0; i < quantidade_tripulantes; i++) {
        tripulante[i].id = i;
        tripulante[i].x = 75;
        tripulante[i].y = 10 + i * 10;
        tripulante[i].pedido_id = -1;
        tripulante[i].tem_pedido = 0;
        pthread_mutex_init(&tripulante[i].lock, NULL);
        pthread_cond_init(&tripulante[i].cond, NULL);
    }
}

void jogar() {

    jogo_esta_valendo = 1;

    nodelay(stdscr, TRUE);

    pthread_t mural_de_ped;
    pthread_t exib_info;
    pthread_t trip[quantidade_tripulantes];
    pthread_t chefe_da_coz;

    // Alocando memória para a quantidade correta de tripulantes
    tripulante = malloc(sizeof(Tripulante) * quantidade_tripulantes);
    if (!tripulante) {
        perror("Erro ao alocar tripulantes");
        exit(1);
    }

    inicializar_tripulantes();

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

    pthread_join(mural_de_ped, NULL);
    pthread_join(exib_info, NULL);
    for (int i = 0; i < quantidade_tripulantes; i++) {
        pthread_join(trip[i], NULL);
    }
    pthread_join(chefe_da_coz, NULL);

}