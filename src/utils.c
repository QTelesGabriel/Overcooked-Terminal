#include "../include/utils.h"
#include "../include/jogar.h"
#include "../include/config.h"
#include "../include/dificuldade.h"

void desenhar_tela() {
    move(0, 0);
    clear();

    attron(COLOR_PAIR(4));
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 116; j++) {
            mvprintw(i, j, " ");
        }
    }

    attron(COLOR_PAIR(3));
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 116; j++) {
            if (i == 0 || 
                i == 50-1 ||
                j == 0 ||
                j == 116-1
            ) {
                mvprintw(i, j, " ");
            }
        }
    }

    attron(COLOR_PAIR(1));
}

void print_logo() {

    mvprintw(2, 2, " _____                                               __                 __     ");
    mvprintw(3, 2, "/\\  __`\\                                            /\\ \\               /\\ \\    ");
    mvprintw(4, 2, "\\ \\ \\/\\ \\  __  __     __   _ __   ___    ___     ___\\ \\ \\/'\\      __   \\_\\ \\   ");
    mvprintw(5, 2, " \\ \\ \\ \\ \\/\\ \\/\\ \\  /'__`\\/\\`'__\\/'___\\ / __`\\  / __`\\ \\ , <    /'__`\\ /'_` \\  ");
    mvprintw(6, 2, "  \\ \\ \\_\\ \\ \\ \\_/ |/\\  __/\\ \\ \\//\\ \\__//\\ \\L\\ \\/\\ \\L\\ \\ \\ \\\\`\\ /\\  __//\\ \\L\\ \\ ");
    mvprintw(7, 2, "   \\ \\_____\\ \\___/ \\ \\____\\\\ \\_\\\\ \\____\\ \\____/\\ \\____/\\ \\_\\ \\_\\ \\____\\ \\___,_\\");
    mvprintw(8, 2, "    \\/_____/\\/__/   \\/____/ \\/_/ \\/____/\\/___/  \\/___/  \\/_/\\/_/\\/____/\\/__,_ /");
    
}

void mostrar_tutorial() {
    desenhar_tela();
    mvprintw(2, 2, "Aperte enter para voltar");
    getch();

    desenhar_tela();
    print_logo();
    mvprintw(12, 2, "1) Jogar");
    mvprintw(13, 2, "2) Tutorial");
    mvprintw(14, 2, "3) Sair");
}

void menu() {    
    desenhar_tela();
    print_logo();
    mvprintw(12, 2, "1) Jogar");
    mvprintw(13, 2, "2) Tutorial");
    mvprintw(14, 2, "3) Sair");

    char op;
    int lin = 16;
    do {
        mvprintw(lin, 2, "-> ");
        op = getch();
        switch(op) {
            case '1':
                escolher_dificuldade();
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
}
