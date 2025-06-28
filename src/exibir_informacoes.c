#include "../include/exibir_informacoes.h"
#include "../include/mural_de_pedidos.h"
#include "../include/config.h"

void definir_espaco_tela(int linhas, int colunas) {
    pthread_mutex_lock(&ncurses_lock);
    attron(COLOR_PAIR(4));
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            mvprintw(i, j, " ");
        }
    }

    attron(COLOR_PAIR(3));
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (i == 0 || 
                i == linhas-1 ||
                (i == 14 && j >= 85) || 
                j == 0 || 
                j == colunas-1 ||
                j == 85
            ) {
                mvprintw(i, j, " ");
            }
        }
    }

    attron(COLOR_PAIR(5));
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if ((i >= 15 && i <= linhas-2) &&
                (j >= 86 && j <= colunas-2)
            ) {
                mvprintw(i, j, " ");
            }
        }
    }

    refresh();
    pthread_mutex_unlock(&ncurses_lock);

}

void mostrar_pedidos(ListaDePedidos *lista) {
    pthread_mutex_lock(&ncurses_lock);
    attron(COLOR_PAIR(2));
    mvprintw(15, 86, "-----------------------------");
    mvprintw(16, 86, "           Pedidos           ");
    mvprintw(17, 86, "-----------------------------");
    pthread_mutex_unlock(&ncurses_lock);

    int lin_ped = 20;
    int col_ped = 90;

    pthread_mutex_lock(&lista_lock);
    Pedido *atual = lista ? lista->primeiro : NULL;
    while (atual != NULL) {
        if (lin_ped >= 50) break;
        pthread_mutex_lock(&ncurses_lock);
        mvprintw(lin_ped, col_ped, "Pedido %d: %s", atual->id, atual->comida.tipo);
        pthread_mutex_unlock(&ncurses_lock);
        lin_ped += 2;
        atual = atual->proximo;
    }
    pthread_mutex_unlock(&lista_lock);

    pthread_mutex_lock(&ncurses_lock);
    refresh();
    pthread_mutex_unlock(&ncurses_lock);

}

void mostrar_chefe() {
    pthread_mutex_lock(&ncurses_lock);
    attron(COLOR_PAIR(7));
    mvprintw(chefe.y - 1, chefe.x - 2, "     ");
    mvprintw(chefe.y, chefe.x - 2, "  C  ");
    mvprintw(chefe.y + 1, chefe.x - 2, "     ");

    refresh();
    pthread_mutex_unlock(&ncurses_lock);
}

void mostrar_tripulantes() {
    pthread_mutex_lock(&ncurses_lock);
    for (int i = 0; i < quantidade_tripulantes; i ++) {
        Tripulante *t = &tripulante[i];
        if(t->tem_pedido == 0) attron(COLOR_PAIR(9));
        else attron(COLOR_PAIR(8));
        mvprintw(t->y, t->x -1, " T ");
    }
    pthread_mutex_unlock(&ncurses_lock);
}

void exibir_informacoes() {
    pthread_mutex_lock(&ncurses_lock);
    noecho();
    pthread_mutex_unlock(&ncurses_lock);

    while(jogo_esta_valendo) {
        definir_espaco_tela(50, 116);
        mostrar_pedidos(lista);
        mostrar_chefe();
        mostrar_tripulantes();
        move(51, 117);
        usleep(10 * 1000);
    }
}
