#include "../include/utils.h"
#include "../include/jogar.h"

void print_logo() {
    printw(" ______   __   __   ______   ______    ______   ______   ______   ___   ___   ______   ______      \n");
    printw("/_____/\\ /_/\\ /_/\\ /_____/\\ /_____/\\  /_____/\\ /_____/\\ /_____/\\ /___/\\/__/\\ /_____/\\ /_____/\\     \n");
    printw("\\:::_ \\ \\\\:\\ \\\\ \\ \\\\::::_\\/_\\:::_ \\ \\ \\:::__\\/ \\:::_ \\ \\\\:::_ \\ \\\\::.\\ \\\\ \\ \\\\::::_\\/_\\:::_ \\ \\    \n");
    printw(" \\:\\ \\ \\ \\\\:\\ \\\\ \\ \\\\:\\/___/\\\\:(_) ) )_\\:\\ \\  __\\:\\ \\ \\ \\\\:\\ \\ \\ \\\\:: \\/_) \\ \\\\:\\/___/\\\\:\\ \\ \\ \\   \n");
    printw("  \\:\\ \\ \\ \\\\:\\_/.:\\ \\\\::___\\/_\\: __ `\\ \\\\:\\ \\/_/\\\\:\\ \\ \\ \\\\:\\ \\ \\ \\\\:. __  ( ( \\::___\\/_\\:\\ \\ \\ \\  \n");
    printw("   \\:\\_\\ \\ \\\\ ..::/ / \\:\\____/\\\\ \\ `\\ \\ \\\\:\\_\\ \\ \\\\:\\_\\ \\ \\\\:\\_\\ \\ \\\\: \\ )  \\ \\ \\:\\____/\\\\:\\/.:| | \n");
    printw("    \\_____\\/ \\___/_(   \\_____\\/ \\_\\/ \\_\\/ \\_____\\/ \\_____\\/ \\_____\\/ \\__\\/\\__\\/  \\_____\\/ \\____/_/ \n\n\n");
}

void mostrar_tutorial() {
    clear();
    move(0, 0);
    printw("Aperte enter para voltar");
    getch();

    clear();
    move(0, 0);
    print_logo();
    printw("1) Jogar\n2) Tutorial\n3) Sair\n\n");
}

void menu() {
    initscr();              // Inicia ncurses
    echo();                 // Não mostrar teclas pressionadas
    cbreak();               // Desativa o buffering de linha
    start_color();
    keypad(stdscr, TRUE);   // Habilita teclas como setas

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);

    attron(COLOR_PAIR(1));
    
    move(0, 0);
    print_logo();
    printw("1) Jogar\n2) Tutorial\n3) Sair\n\n");

    char op;
    int lin = 13;
    do {
        move(lin, 0);
        printw("-> ");
        op = getch();
        switch(op) {
            case '1':
                jogar(3);
                break;

            case '2':
                mostrar_tutorial();
                lin = 13;
                break;

            case '3':
                printw("Saindo...");
                break;
            
            default:
                lin += 2;
                printw("\nDigite uma opção válida\n");
                break;
        }

    } while (op != '3');
    
    refresh();              // Atualiza a tela com o conteúdo desenhado

    endwin();               // Finaliza a ncurses e retorna ao terminal normal
}
