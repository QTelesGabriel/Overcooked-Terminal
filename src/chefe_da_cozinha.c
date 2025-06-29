#include "../include/chefe_da_cozinha.h"
#include "../include/config.h"

int colide_com_algo(int novo_y, int novo_x) {
    // Coordenadas da área do chefe
    int chefe_top = novo_y - 1;
    int chefe_bot = novo_y + 1;
    int chefe_esq = novo_x - 2;
    int chefe_dir = novo_x + 2;

    for (int i = 0; i < quantidade_tripulantes; i++) {
        int t_y = tripulante[i].y;
        int t_x = tripulante[i].x;
        int c_y = bancada_cozinha[i].y;
        int c_x = bancada_cozinha[i].x;
        int p_y = bancada_preparo[i].y;
        int p_x = bancada_preparo[i].x;

        // Coordenadas do que não pode colidir
        int trip_top = t_y;
        int trip_bot = t_y;
        int trip_esq = t_x - 1;
        int trip_dir = t_x + 1;

        int coz_top = c_y - 2;
        int coz_bot = c_y + 2;
        int coz_esq = c_x - 4;
        int coz_dir = c_x + 4;

        int prep_top = p_y - 2;
        int prep_bot = p_y + 2;
        int prep_esq = p_x - 4;
        int prep_dir = p_x + 4;

        // Verifica sobreposição dos retângulos
        if (
            (chefe_dir >= trip_esq &&
            chefe_esq <= trip_dir &&
            chefe_bot >= trip_top &&
            chefe_top <= trip_bot) ||

            (chefe_dir >= coz_esq &&
            chefe_esq <= coz_dir &&
            chefe_bot >= coz_top &&
            chefe_top <= coz_bot) ||

            (chefe_dir >= prep_esq &&
            chefe_esq <= prep_dir &&
            chefe_bot >= prep_top &&
            chefe_top <= prep_bot)
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

            case ' ': { // Começar a dar ordem para algum tripulante
                char numero_buffer[10] = {0};
                int numero_index = 0;
                char ch = 0;

                while (1) {
                    pthread_mutex_lock(&ncurses_lock);
                    ch = getch();
                    pthread_mutex_unlock(&ncurses_lock);

                    if (isdigit(ch)) {
                        if (numero_index < sizeof(numero_buffer) - 1) {
                            numero_buffer[numero_index++] = ch;
                            numero_buffer[numero_index] = '\0';
                        }
                    } else if (ch == ' ') {
                        break; // fim da digitação
                    }
                }

                if (numero_index > 0) {
                    int numero_digitado = atoi(numero_buffer);

                    pthread_mutex_lock(&id_pedido_lock);
                    id_pedido_que_sera_feito = numero_digitado;
                    pthread_mutex_unlock(&id_pedido_lock);

                    int existe = 0;
                    pthread_mutex_lock(&lista_lock);
                    Pedido *atual = lista->primeiro;
                    while (atual) {
                        if (atual->id == numero_digitado) {
                            existe = 1;
                            break;
                        }
                        atual = atual->proximo;
                    }
                    pthread_mutex_unlock(&lista_lock);

                    if (!existe) {
                        break;
                    }

                    // Verificar se existe tripulante com esse ID
                    for (int i = 0; i < quantidade_tripulantes; i++) {
                        if (
                            tripulante[i].x + 2 >= chefe.x - 2 &&
                            tripulante[i].x - 2 <= chefe.x + 2 &&
                            tripulante[i].y + 1 >= chefe.y - 1 &&
                            tripulante[i].y - 1 <= chefe.y + 1 
                        ) {
                            // Enviar sinal para o tripulante correspondente
                            pthread_mutex_lock(&tripulante[i].lock);
                            tripulante[i].pedido_id = id_pedido_que_sera_feito;
                            tripulante[i].tem_pedido = 1;
                            pthread_cond_signal(&tripulante[i].cond);
                            pthread_mutex_unlock(&tripulante[i].lock);

                            break;
                        }
                    }
                }

                break;
            }

            case ERR:
                break;

            case 'p':
                jogo_esta_valendo = 0;
                break;

            case 'P':
                jogo_esta_valendo = 0;
                break;

            default:        // Algum outro botão;
                break;
        }

        if (!colide_com_algo(novo_y, novo_x)) {
            chefe.x = novo_x;
            chefe.y = novo_y;
        } else {
            novo_x = chefe.x;
            novo_y = chefe.y;
        }
    }
}