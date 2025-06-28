#ifndef EXIBIR_INFORMACOES_H
#define EXIBIR_INFORMACOES_H

#include "./mural_de_pedidos.h"
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void definir_espaco_tela(int linhas, int colunas);
void mostrar_pedidos(ListaDePedidos *lista);
void mostrar_preparo();
void mostrar_cozinha();
void mostrar_chefe();
void mostrar_tripulantes();
void exibir_informacoes();

#endif