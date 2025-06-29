#include "../include/mural_de_pedidos.h"
#include "../include/config.h"

void inicializar_lista(ListaDePedidos *lista) {
    lista->size = 0;
    lista->primeiro = NULL;
}

Pedido* criar_pedido(Comida comida, int id) {
    Pedido *novo = (Pedido*)malloc(sizeof(Pedido));
    if (!novo) {
        perror("Erro ao alocar memória para o pedido");
        exit(1);
    }
    novo->comida = comida;
    novo->id = id;
    novo->proximo = NULL;
    return novo;
}

void adicionar_pedido(ListaDePedidos *lista, Comida comida) {
    Pedido *novo = criar_pedido(comida, proximo_id++);

    if (lista->primeiro == NULL) {
        lista->primeiro = novo;
    } else {
        Pedido *atual = lista->primeiro;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }

    lista->size++;
}

void remover_pedido(ListaDePedidos *lista, int id) {
    if (lista->primeiro == NULL) return;

    Pedido *atual = lista->primeiro;
    Pedido *anterior = NULL;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) return;

    if (anterior == NULL) {
        lista->primeiro = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    lista->size--;
}

void liberar_lista(ListaDePedidos *lista) {
    Pedido *atual = lista->primeiro;
    while (atual != NULL) {
        Pedido *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    lista->primeiro = NULL;
    lista->size = 0;
}

void mural_de_pedidos(int tempo_por_pedido) {

    pthread_mutex_lock(&lista_lock);
    lista = (ListaDePedidos *)malloc(sizeof(ListaDePedidos));
    if (!lista) {
        perror("Erro ao alocar ListaDePedidos");
        exit(1);
    }
    inicializar_lista(lista);
    pthread_mutex_unlock(&lista_lock);

    while (jogo_esta_valendo) {
        int indice = rand() % total_pratos;
        Comida comida = pratos[indice];

        pthread_mutex_lock(&lista_lock);
        adicionar_pedido(lista, comida);
        pthread_mutex_unlock(&lista_lock);

        usleep(tempo_por_pedido * 1000 * 1000);
    }

}
