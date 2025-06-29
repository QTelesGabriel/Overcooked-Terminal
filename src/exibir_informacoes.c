#include "../include/exibir_informacoes.h"
#include "../include/mural_de_pedidos.h"
#include "../include/utils.h"
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
    for (int i = 0; i < quantidade_tripulantes; i++) {
        Tripulante *t = &tripulante[i];
        if(t->tem_pedido == 0) attron(COLOR_PAIR(9));
        else attron(COLOR_PAIR(8));
        mvprintw(t->y, t->x -1, " T ");
    }
    pthread_mutex_unlock(&ncurses_lock);
}

void mostrar_preparo() {
    pthread_mutex_lock(&ncurses_lock);
    for (int i = 0; i < quantidade_tripulantes; i++) {
        BancadaPreparo *bp = &bancada_preparo[i];
        if(bp->ocupada == 0) attron(COLOR_PAIR(10));
        else attron(COLOR_PAIR(11));
        mvprintw(bp->y - 2, bp-> x - 4, "         ");
        mvprintw(bp->y - 1, bp-> x - 4, "         ");
        mvprintw(bp->y, bp->x - 4, " Preparo ");
        mvprintw(bp->y + 1, bp-> x - 4, "         ");
        mvprintw(bp->y + 2, bp-> x - 4, "         ");
    }
    pthread_mutex_unlock(&ncurses_lock);
}

void mostrar_cozinha() {
    pthread_mutex_lock(&ncurses_lock);
    for (int i = 0; i < quantidade_tripulantes; i++) {
        BancadaCozinha *bc = &bancada_cozinha[i];
        if(bc->ocupada == 0) attron(COLOR_PAIR(10));
        else attron(COLOR_PAIR(11));
        mvprintw(bc->y - 2, bc-> x - 4, "         ");
        mvprintw(bc->y - 1, bc-> x - 4, "         ");
        mvprintw(bc->y, bc->x - 4, " Cozinha ");
        mvprintw(bc->y + 1, bc-> x - 4, "         ");
        mvprintw(bc->y + 2, bc-> x - 4, "         ");
    }
    pthread_mutex_unlock(&ncurses_lock);
}

void mostrar_informacoes() {
    pthread_mutex_lock(&ncurses_lock);
    attron(COLOR_PAIR(1));
    mvprintw(2, 87, "Tempo restante: %d", tempo_de_jogo);
    mvprintw(4, 87, "Pedidos entregues: %d", pontuacao);
    mvprintw(5, 87, "Pedidos ativos: %d", lista->size);
    mvprintw(7, 87, "Tempo dos pratos:");
    for (int i = 0; i < total_pratos; i++) {
        mvprintw(8 + i, 91, "%10s: P-%ds | C-%ds", pratos[i].tipo, pratos[i].tempo_preparo, pratos[i].tempo_cozinha);
    }
    attroff(COLOR_PAIR(1));
    pthread_mutex_unlock(&ncurses_lock);
}

void tela_final() {
    pthread_mutex_lock(&ncurses_lock);
    attron(COLOR_PAIR(1));

    // Título
    mvprintw(8, 35, " ____    ______               ");
    mvprintw(9, 35, "/\\  _`\\ /\\__  _\\   /'\\_/`\\    ");
    mvprintw(10, 35, "\\ \\ \\L\\_\\/_/\\ \\/  /\\      \\   ");
    mvprintw(11, 35, " \\ \\  _\\/  \\ \\ \\  \\ \\ \\__\\ \\  ");
    mvprintw(12, 35, "  \\ \\ \\/    \\_\\ \\__\\ \\ \\_/\\ \\ ");
    mvprintw(13, 35, "   \\ \\_\\    /\\_____\\\\ \\_\\\\ \\_\\");
    mvprintw(14, 35, "    \\/_/    \\/_____/ \\/_/ \\/_/");

    // Pontuação
    mvprintw(18, 40, "Pedidos Completos: %d", pontuacao);
    mvprintw(20, 40, "Faltaram: %d", lista->size);
    
    // Mensagem de retorno
    mvprintw(26, 28, "PRESSIONE QUALQUER TECLA PARA VOLTAR AO MENU");

    attroff(COLOR_PAIR(1));
    refresh();
    pthread_mutex_unlock(&ncurses_lock);

    // Espera uma tecla antes de voltar
    nodelay(stdscr, FALSE); // Espera input
    getch();                // Aguarda pressionar
    nodelay(stdscr, TRUE);  // Retorna ao modo não bloqueante
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
        mostrar_preparo();
        mostrar_cozinha();
        mostrar_informacoes();
        usleep(10 * 1000);
    }

    desenhar_tela();
    tela_final();
}
