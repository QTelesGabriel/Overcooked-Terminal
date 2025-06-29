#include "../include/utils.h"

int main() {
    
    srand(time(NULL));
    
    initscr();              // Inicia ncurses
    echo();                 // Mostrar teclas pressionadas
    cbreak();               // Desativa o buffering de linha
    start_color();
    keypad(stdscr, TRUE);   // Habilita teclas como setas
    nodelay(stdscr, FALSE);

    init_pair(1,  COLOR_GREEN,   COLOR_BLACK);    // Painel lateral de informações e tela final
    init_pair(2,  COLOR_RED,     COLOR_YELLOW);   // Título da seção "Pedidos"
    init_pair(3,  COLOR_WHITE,   COLOR_WHITE);    // Borda da tela (invisível/escura)
    init_pair(4,  COLOR_BLACK,   COLOR_BLACK);    // Fundo branco da tela principal
    init_pair(5,  COLOR_YELLOW,  COLOR_YELLOW);   // Fundo da seção dos pedidos (à direita)
    init_pair(6,  COLOR_WHITE,   COLOR_BLACK);    // (não foi utilizado diretamente)
    init_pair(7,  COLOR_GREEN,   COLOR_RED);      // Chefe de cozinha em estado normal
    init_pair(8,  COLOR_WHITE,   COLOR_BLUE);     // Tripulante que está com um pedido ativo
    init_pair(9,  COLOR_WHITE,   COLOR_CYAN);     // Tripulante que está ocioso (sem pedido)
    init_pair(10, COLOR_WHITE,   COLOR_GREEN);    // Bancada (preparo/cozinha) **livre**
    init_pair(11, COLOR_WHITE,   COLOR_MAGENTA);  // Bancada (preparo/cozinha) **ocupada**

    menu();

    endwin();               // Finaliza a ncurses e retorna ao terminal normal

    return 0;

}