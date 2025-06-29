#include "../include/tripulantes.h"
#include "../include/mural_de_pedidos.h"
#include "../include/config.h"

#include <string.h>

int colide_area_ampla(int x, int y) {
    int t_top = y - 4, t_bot = y + 4;
    int t_esq = x - 4, t_dir = x + 4;

    int chefe_top = chefe.y - 3, chefe_bot = chefe.y + 3;
    int chefe_esq = chefe.x - 3, chefe_dir = chefe.x + 3;
    if (
        t_dir >= chefe_esq &&
        t_esq <= chefe_dir &&
        t_bot >= chefe_top &&
        t_top <= chefe_bot
    ) return 1;

    for (int i = 0; i < quantidade_tripulantes; i++) {
        int o_x = tripulante[i].x;
        int o_y = tripulante[i].y;
        int ot_top = o_y - 3, ot_bot = o_y + 3;
        int ot_esq = o_x - 3, ot_dir = o_x + 3;

        if (
            t_dir >= ot_esq &&
            t_esq <= ot_dir &&
            t_bot >= ot_top &&
            t_top <= ot_bot
        ) return 1;
    }

    return 0;
}

int definir_destino(Tripulante *tripulante, const char *tipo) {
    int id = 0;
    if (strcmp(tipo, "preparo") == 0) {
        pthread_mutex_lock(&preparo_lock);
        for (int i = 0; i < quantidade_tripulantes; i++) {
            if (!bancada_preparo[i].ocupada) {
                tripulante->destino_x = bancada_preparo[i].x;
                tripulante->destino_y = bancada_preparo[i].y + 3;
                bancada_preparo[i].ocupada = 1;
                id = i;
                break;
            }
        }
        pthread_mutex_unlock(&preparo_lock);
    } else if (strcmp(tipo, "cozinha") == 0) {
        pthread_mutex_lock(&cozinha_lock);
        for (int i = quantidade_tripulantes-1; i >= 0; i--) {
            if (!bancada_cozinha[i].ocupada) {
                tripulante->destino_x = bancada_cozinha[i].x + 6;
                tripulante->destino_y = bancada_cozinha[i].y;
                bancada_cozinha[i].ocupada = 1;
                id = i;
                break;
            }
        }
        pthread_mutex_unlock(&cozinha_lock);
    } else if (strcmp(tipo, "fim") == 0) {
        int x, y, tentativas = 0;
        do {
            x = 30 + rand() % (75 - 30);
            y = 20 + rand() % (40 - 20);
            tentativas++;
        } while (colide_area_ampla(x, y) && tentativas < 100);
        tripulante->destino_x = x;
        tripulante->destino_y = y;
    } else {
        printf("Não é válido\n");
    }

    return id;
}

int tempo_de_espera(int id, const char *tipo) {
    int espera;
    
    pthread_mutex_lock(&lista_lock);
    Pedido *atual = lista->primeiro;
    while (atual && atual->id != id) {
        atual = atual->proximo;
    }

    if (atual) {
        if (strcmp(tipo, "preparo") == 0) {
            espera = atual->comida.tempo_preparo;
        } else if (strcmp(tipo, "cozinha") == 0) {
            espera = atual->comida.tempo_cozinha;
        } else {
            printf("Não é válido\n");
        }
    }

    pthread_mutex_unlock(&lista_lock);

    return espera;
}

int colide_com_tripulantes_ou_chefe(int x, int y, Tripulante *t) {
    // Verifica colisão com chefe
    int chefe_top = chefe.y - 1, chefe_bot = chefe.y + 1;
    int chefe_esq = chefe.x - 2, chefe_dir = chefe.x + 2;
    int t_top = y, t_bot = y;
    int t_esq = x - 1, t_dir = x + 1;

    if (
        t_dir >= chefe_esq &&
        t_esq <= chefe_dir &&
        t_bot >= chefe_top &&
        t_top <= chefe_bot
    ) return 1;

    // Verifica colisão com outros tripulantes
    for (int i = 0; i < quantidade_tripulantes; i++) {
        if (&tripulante[i] == t) continue;
        int o_x = tripulante[i].x;
        int o_y = tripulante[i].y;
        int ot_top = o_y - 1, ot_bot = o_y + 1;
        int ot_esq = o_x - 1, ot_dir = o_x + 1;

        if (
            t_dir >= ot_esq &&
            t_esq <= ot_dir &&
            t_bot >= ot_top &&
            t_top <= ot_bot
        ) return 1;
    }

    return 0;
}

void mover_tripulante(Tripulante *t) {
    while (t->x != t->destino_x || t->y != t->destino_y) {
        int dx = t->destino_x - t->x;
        int dy = t->destino_y - t->y;

        int direcao_preferida = rand() % 2; // 0: x primeiro, 1: y primeiro

        int opcoes[2][2];

        if (direcao_preferida == 0) {
            opcoes[0][0] = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
            opcoes[0][1] = 0;
            opcoes[1][0] = 0;
            opcoes[1][1] = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
        } else {
            opcoes[0][0] = 0;
            opcoes[0][1] = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
            opcoes[1][0] = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
            opcoes[1][1] = 0;
        }

        int moveu = 0;
        for (int i = 0; i < 2; i++) {
            int novo_x = t->x + opcoes[i][0];
            int novo_y = t->y + opcoes[i][1];

            if (!colide_com_tripulantes_ou_chefe(novo_x, novo_y, t) &&
                novo_x >= 3 && novo_x <= 82 &&
                novo_y >= 2 && novo_y <= 47
            ) {
                t->x = novo_x;
                t->y = novo_y;
                moveu = 1;
                break;
            }
        }

        if (!moveu) {
            // Tenta aleatoriamente em até 4 direções diferentes
            int desvios[4][2] = {
                {0, -1},  // cima
                {0,  1},  // baixo
                {-1, 0},  // esquerda
                {1,  0}   // direita
            };

            // Embaralha a ordem das direções
            for (int i = 0; i < 4; i++) {
                int j = rand() % 4;
                int tmp0 = desvios[i][0], tmp1 = desvios[i][1];
                desvios[i][0] = desvios[j][0];
                desvios[i][1] = desvios[j][1];
                desvios[j][0] = tmp0;
                desvios[j][1] = tmp1;
            }

            for (int i = 0; i < 4; i++) {
                int novo_x = t->x + desvios[i][0];
                int novo_y = t->y + desvios[i][1];

                if (!colide_com_tripulantes_ou_chefe(novo_x, novo_y, t) &&
                    novo_x >= 3 && novo_x <= 82 &&
                    novo_y >= 2 && novo_y <= 47
                ) {
                    t->x = novo_x;
                    t->y = novo_y;
                    moveu = 1;
                    break;
                }
            }
        } 
        
        if (!moveu) {
            usleep(1 * 1000);
        } else {
            usleep(30 * 1000); // tempo de movimento
        }
    }
}

void tripulantes(Tripulante *tripulante) {
    Tripulante *t = tripulante;
    while (jogo_esta_valendo) {
        pthread_mutex_lock(&t->lock);
        while (!t->tem_pedido && jogo_esta_valendo) {
            pthread_cond_wait(&t->cond, &t->lock);
        }
        pthread_mutex_unlock(&t->lock);

        if (!jogo_esta_valendo) break;
        
        pthread_mutex_lock(&id_pedido_lock);
        t->pedido_id = id_pedido_que_sera_feito;
        pthread_mutex_unlock(&id_pedido_lock);

        if (!jogo_esta_valendo) break;

        int id_bancada_preparo = definir_destino(t, "preparo");
        mover_tripulante(t);
        for (int i = 0; i < tempo_de_espera(t->pedido_id, "preparo"); i++) {
            if (!jogo_esta_valendo) break;
            usleep(1000 * 1000);
        }
        bancada_preparo[id_bancada_preparo].ocupada = 0;     
        
        if (!jogo_esta_valendo) break;

        int id_bancada_cozinha = definir_destino(t, "cozinha");
        mover_tripulante(t);
        for (int i = 0; i < tempo_de_espera(t->pedido_id, "cozinha"); i++) {
            if (!jogo_esta_valendo) break;
            usleep(1000 * 1000);
        }
        bancada_cozinha[id_bancada_cozinha].ocupada = 0;
        
        if (!jogo_esta_valendo) break;

        remover_pedido(lista, t->pedido_id);

        pontuacao++;

        if (!jogo_esta_valendo) break;

        definir_destino(t, "fim");
        mover_tripulante(t);

        if (!jogo_esta_valendo) break;

        t->tem_pedido = 0;
    }
}