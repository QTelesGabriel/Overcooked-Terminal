#include "../include/chefe_da_cozinha.h"
#include "../include/config.h"

int colide_com_tripulante(int novo_y, int novo_x) {
    // Coordenadas da área do chefe
    int chefe_top = novo_y - 1;
    int chefe_bot = novo_y + 1;
    int chefe_esq = novo_x - 2;
    int chefe_dir = novo_x + 2;

    for (int i = 0; i < quantidade_tripulantes; i++) {
        int t_y = tripulante[i].y;
        int t_x = tripulante[i].x;

        // Coordenadas da área do tripulante
        int trip_top = t_y;
        int trip_bot = t_y;
        int trip_esq = t_x - 1;
        int trip_dir = t_x + 1;

        // Verifica sobreposição dos retângulos
        if (
            chefe_dir >= trip_esq &&
            chefe_esq <= trip_dir &&
            chefe_bot >= trip_top &&
            chefe_top <= trip_bot
        ) {
            return 1; // Colidiu
        }
    }

    return 0; // Sem colisão
}

void chefe_da_cozinha() {
    
    int movimento;

    int novo_x = chefe.x;
    int novo_y = chefe.y;

    while (jogo_esta_valendo) {

        pthread_mutex_lock(&ncurses_lock);
        movimento = getch();
        pthread_mutex_unlock(&ncurses_lock);

        switch (movimento) {
            case KEY_UP:    // Movimentar para Cima
                if (chefe.y > 2) novo_y--;
                break;
            
            case KEY_DOWN:  // Movimentar para Baixo
                if (chefe.y < 47) novo_y++;
                break;

            case KEY_LEFT:  // Movimentar para a Esquerda
                if (chefe.x > 3) novo_x--;
                break;

            case KEY_RIGHT: // Movimentar para a Direita
                if (chefe.x < 82) novo_x++;
                break;

            case ' ':       // Começar a dar ordem para algum tripulante
                break;

            case ERR:
                break;

            default:        // Algum outro botão;
                break;
        }

        if (!colide_com_tripulante(novo_y, novo_x)) {
            chefe.x = novo_x;
            chefe.y = novo_y;
        } else {
            novo_x = chefe.x;
            novo_y = chefe.y;
        }
    }
}