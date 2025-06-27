#include "../include/jogar.h"
#include "../include/mural_de_pedidos.h"
#include "../include/exibir_informacoes.h"
#include "../include/tripulantes.h"
#include "../include/chefe_da_cozinha.h"

#include <ncurses.h>

int jogo_esta_valendo;

void definir_espaco_tela(int linhas, int colunas) {
    attron(COLOR_PAIR(4));
    for(int i = 0; i < linhas; i++) {
        for(int j = 0; j < colunas; j++) {
            mvprintw(i, j, " ");
        }
    }

    refresh();
}

void* thread_mural_de_pedidos(void* arg) {
    mural_de_pedidos(1000);
}

void* thread_exibir_informacoes(void* arg) {
    exibir_informacoes();
}

void* thread_tripulantes(void* arg) {
    tripulantes();
}

void* thread_chefe_da_cozinha(void* arg) {
    chefe_da_cozinha();
}

void jogar(int qtd_tripulantes) {

    definir_espaco_tela(40, 116);

    jogo_esta_valendo = 1;

    pthread_t mural_de_ped;
    pthread_t exib_info;
    pthread_t trip[qtd_tripulantes];
    pthread_t chefe_da_coz;

    // Cria thread Mural de pedidos
    pthread_create(&mural_de_ped, NULL, thread_mural_de_pedidos, NULL);
    // Cria thread Exibir Informações
    pthread_create(&exib_info, NULL, thread_exibir_informacoes, NULL);
    // Cria N threads Tripulantes
    for (int i = 0; i < qtd_tripulantes; i++) {
        pthread_create(&trip[i], NULL, thread_tripulantes, NULL);
    }
    // Cria thread Chefe da Cozinha
    pthread_create(&chefe_da_coz, NULL, thread_chefe_da_cozinha, NULL);

    pthread_join(mural_de_ped, NULL);
    pthread_join(exib_info, NULL);
    for (int i = 0; i < qtd_tripulantes; i++) {
        pthread_join(trip[i], NULL);
    }
    pthread_join(chefe_da_coz, NULL);

}