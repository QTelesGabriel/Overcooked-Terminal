#include "../include/utils.h"

int main() {
    
    srand(time(NULL));
    
    initscr();              // Inicia ncurses
    echo();                 // Mostrar teclas pressionadas
    cbreak();               // Desativa o buffering de linha
    start_color();
    keypad(stdscr, TRUE);   // Habilita teclas como setas
    nodelay(stdscr, FALSE);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_RED);
    init_pair(8, COLOR_WHITE, COLOR_BLUE);
    init_pair(9, COLOR_WHITE, COLOR_CYAN);
    init_pair(10, COLOR_WHITE, COLOR_GREEN);
    init_pair(11, COLOR_WHITE, COLOR_MAGENTA);

    menu();

    endwin();               // Finaliza a ncurses e retorna ao terminal normal

    return 0;

}