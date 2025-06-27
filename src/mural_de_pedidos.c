#include "../include/mural_de_pedidos.h"

int proximo_id = 1;

// Inicializa a lista vazia
void inicializar_lista(ListaDePedidos *lista) {
    lista->size = 0;
    lista->primeiro = NULL;
}

// Cria um novo pedido
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

// Adicionar um novo pedido a lista de pedidos
void adicionar_pedido(ListaDePedidos *lista, Comida comida) {
    Pedido *novo = criar_pedido(comida, proximo_id);
    proximo_id++;

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

// Remover um pedido
void remover_pedido(ListaDePedidos *lista, int id) {
    if (lista->primeiro == NULL) {
        fprintf(stderr, "Lista vazia! Não é possível remover.\n");
        exit(1);
    }

    Pedido *atual = lista->primeiro;
    Pedido *anterior = NULL;

    while(atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        fprintf(stderr, "Pedido com id %d, não encontrado.\n", id);
        return;
    }

    // Caso seja o primeiro da lista
    if (anterior == NULL) {
        lista->primeiro = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    lista->size--;
}

// Mostra todos os pedidos da lista
void mostrar_pedidos(ListaDePedidos *lista) {

    attron(COLOR_PAIR(2));
    mvprintw(15, 86, "------------------------");
    mvprintw(16, 86, "         Pedidos        ");
    mvprintw(17, 86, "------------------------");
    refresh();

    int lin_ped = 20;
    int col_ped = 86;

    Pedido *atual = lista->primeiro;
    while (atual != NULL) {
        mvprintw(lin_ped, col_ped, "Pedido %d: Tipo %s", 
            atual->id,
            atual->comida.tipo
        );
        lin_ped += 2;
        atual = atual->proximo;
    }
    refresh();
    
    if (lista->size == 0) {
        printf("Nenhum pedido na lista.\n");
    }
}

// Libera toda a memória da lista
void liberar_lista(ListaDePedidos *lista) {
    while (lista->primeiro != NULL) {
        if (lista->primeiro == NULL) {
            fprintf(stderr, "Lista vazia! Não é possível remover.\n");
            exit(1);
        }

        Pedido *removido = lista->primeiro;

        lista->primeiro = removido->proximo;
        free(removido);
        lista->size--;
    }
}

void mural_de_pedidos(int tempo_por_pedido) {
    // Array de pratos disponíveis
    Comida pratos[] = {
        {"Pizza", 4, 6},  // Pizza
        {"Hamburguer", 3, 5},  // Hamburguer
        {"Sushi", 5, 4},  // Sushi
        {"Taco", 2, 3},  // Tacos
        {"Lasanha", 6, 7}   // Lasanha
    };

    // Criando a lista de pedidos
    ListaDePedidos *lista = (ListaDePedidos *)malloc(sizeof(ListaDePedidos));
    if (!lista) {
        perror("Erro ao alocar ListaDePedidos");
        exit(1);
    }
    inicializar_lista(lista);

    int total_pratos = sizeof(pratos) / sizeof(Comida);
    
    // Começando a mostrar a lista de pedidos
    while (jogo_esta_valendo) {
        int indice = rand() % total_pratos;
        Comida comida = pratos[indice];

        adicionar_pedido(lista, comida);
        mostrar_pedidos(lista);

        usleep(tempo_por_pedido * 1000);
    }

    liberar_lista(lista);
    free(lista);
}