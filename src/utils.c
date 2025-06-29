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

    refresh();
}

void print_logo() {

    attron(COLOR_PAIR(1));
    mvprintw(2, 2, " _____                                               __                 __                           ");
    mvprintw(3, 2, "/\\  __`\\                                            /\\ \\               /\\ \\                          ");
    mvprintw(4, 2, "\\ \\ \\/\\ \\  __  __     __   _ __   ___    ___     ___\\ \\ \\/'\\      __   \\_\\ \\                         ");
    mvprintw(5, 2, " \\ \\ \\ \\ \\/\\ \\/\\ \\  /'__`\\/\\`'__\\/'___\\ / __`\\  / __`\\ \\ , <    /'__`\\ /'_` \\                        ");
    mvprintw(6, 2, "  \\ \\ \\_\\ \\ \\ \\_/ |/\\  __/\\ \\ \\//\\ \\__//\\ \\_\\ \\/\\ \\_\\ \\ \\ \\\\`\\ /\\  __//\\ \\_\\ \\                       ");
    mvprintw(7, 2, "   \\ \\_____\\ \\___/ \\ \\____\\\\ \\_\\\\ \\____\\ \\____/\\ \\____/\\ \\_\\ \\_\\ \\____\\ \\___,_\\                      ");
    mvprintw(8, 2, "    \\/_____/\\/__/   \\/____/ \\/_/ \\/____/\\/___/  \\/___/  \\/_/\\/_/\\/____/\\/__,_ /                      ");
    mvprintw(9, 2, "                                                                                                     ");
    mvprintw(10, 2, "                                                                                                    ");
    mvprintw(11, 2, " ____                                              ____                                             ");
    mvprintw(12, 2, "/\\  _`\\                                           /\\  _`\\                                           ");
    mvprintw(13, 2, "\\ \\ \\/_  ___   _ __    __          ___     ___    \\ \\ \\_/     ____  _____      __      ___    ___   ");
    mvprintw(14, 2, " \\ \\  _\\/ __`\\/\\`'__\\/'__`\\      /' _ `\\  / __`\\   \\ \\  _\\   /',__\\/\\ '__`\\  /'__`\\   /'___\\ / __`\\ ");
    mvprintw(15, 2, "  \\ \\ \\/\\ \\_\\ \\ \\ \\//\\ \\_\\.\\_    /\\ \\/\\ \\/\\ \\ \\ \\   \\ \\ \\/_ /\\__, `\\ \\ \\_\\ \\/\\ \\_\\.\\_/\\ \\__//\\ \\_\\ \\");
    mvprintw(16, 2, "   \\ \\_\\ \\____/\\ \\_\\\\ \\__/.\\_\\   \\ \\_\\ \\_\\ \\____/    \\ \\___\\\\/\\____/\\ \\ ,__/\\ \\__/.\\_\\ \\____\\ \\____/");
    mvprintw(17, 2, "    \\/_/\\/___/  \\/_/ \\/__/\\/_/    \\/_/\\/_/\\/___/      \\/___/ \\/___/  \\ \\ \\/  \\/__/\\/_/\\/____/\\/___/ ");
    mvprintw(18, 2, "                                                                      \\ \\_\\                         ");
    mvprintw(19, 2, "                                                                       \\/_/                         ");
    mvprintw(20, 2, "   _   __        _____                                                                               ");
    mvprintw(21, 2, " /' \\ /\\ `\\     /\\___ \\                                                                              ");
    mvprintw(22, 2, "/\\_, \\\\`\\  \\    \\/__/\\ \\    ___      __      __     _ __                                             ");
    mvprintw(23, 2, "\\/_/\\ \\`\\`\\ \\      _\\ \\ \\  / __`\\  /'_ `\\  /'__`\\  /\\`'__\\                                           ");
    mvprintw(24, 2, "   \\ \\ \\`\\/' \\    /\\ \\_\\ \\/\\ \\_\\ \\/\\ \\_\\ \\/\\ \\_\\.\\_\\ \\ \\/                                            ");
    mvprintw(25, 2, "    \\ \\_\\/\\__/    \\ \\____/\\ \\____/\\ \\____ \\ \\__/.\\_\\\\ \\_\\                                            ");
    mvprintw(26, 2, "     \\/_/\\/_/      \\/___/  \\/___/  \\/____\\ \\/__/\\/_/ \\/_/                                            ");
    mvprintw(27, 2, "                                     /\\____/                                                         ");
    mvprintw(28, 2, "                                     \\_/__/                                                          ");
    mvprintw(29, 2, "   ___    __       ______         __                                ___                              ");
    mvprintw(30, 2, " /'___`\\ /\\ `\\    /\\__  _\\       /\\ \\__                __          /\\_ \\                             ");
    mvprintw(31, 2, "/\\_\\ /\\ \\\\`\\  \\   \\/_/\\ \\/ __  __\\ \\ ,_\\   ___   _ __ /\\_\\     __  \\//\\ \\                            ");
    mvprintw(32, 2, "\\/_/// /__`\\`\\ \\     \\ \\ \\/\\ \\/\\ \\\\ \\ \\/  / __`\\/\\`'__\\/\\ \\  /'__`\\  \\ \\ \\                           ");
    mvprintw(33, 2, "   // /_\\ \\`\\/' \\     \\ \\ \\ \\ \\_\\ \\\\ \\ \\_/\\ \\_\\ \\ \\ \\/ \\ \\ \\/\\ \\_\\.\\_ \\_\\ \\_                         ");
    mvprintw(34, 2, "  /\\______/ /\\__/      \\ \\_\\ \\____/ \\ \\__\\ \\____/\\ \\_\\  \\ \\_\\ \\__/.\\_\\/\\____\\                        ");
    mvprintw(35, 2, "  \\/_____/  \\/_/        \\/_/\\/___/   \\/__/\\/___/  \\/_/   \\/_/\\/__/\\/_/\\/____/                        ");
    mvprintw(36, 2, "                                                                                                     ");
    mvprintw(37, 2, "                                                                                                     ");
    mvprintw(38, 2, "   __    __       ____                                                                               ");
    mvprintw(39, 2, " /'__`\\ /\\ `\\    /\\  _`\\             __                                                              ");
    mvprintw(40, 2, "/\\_\\_\\ \\\\`\\  \\   \\ \\,\\_\\_\\     __   /\\_\\  _ __                                                       ");
    mvprintw(41, 2, "\\/_/_\\_<_`\\`\\ \\   \\/_\\__ \\   /'__`\\ \\/\\ \\/\\`'__\\                                                     ");
    mvprintw(42, 2, "  /\\ \\_\\ \\`\\/' \\    /\\ \\_\\ \\/\\ \\_\\.\\_\\ \\ \\ \\ \\/                                                      ");
    mvprintw(43, 2, "  \\ \\____/ /\\__/    \\ `\\____\\ \\__/.\\_\\\\ \\_\\ \\_\\                                                      ");
    mvprintw(44, 2, "   \\/___/  \\/_/      \\/_____/\\/__/\\/_/ \\/_/\\/_/                                                      ");
    mvprintw(45, 2, "                                                                                                     ");
                                                                                                    
    refresh();
}

void mostrar_tutorial() {
    desenhar_tela();

    attron(COLOR_PAIR(1));
    int lin = 2;
    int col = 4;

    mvprintw(lin++, col, "============================= TUTORIAL =============================");
    mvprintw(lin++, col, "Bem-vindo, Chefe de Cozinha! Seu objetivo é preparar");
    mvprintw(lin++, col, "os pedidos que aparecem no mural e entregá-los com");
    mvprintw(lin++, col, "agilidade e precisão antes que o tempo acabe.");
    lin++;
    
    mvprintw(lin++, col, "Controles do Chefe:");
    lin++;
    mvprintw(lin++, col, "  P - Sair do jogo imediatamente");
    lin++;
    mvprintw(lin++, col, "  Seta para CIMA - Movimentar o chefe para cima");
    mvprintw(lin++, col, "  Seta para BAIXO - Movimentar o chefe para baixo");
    mvprintw(lin++, col, "  Seta para DIREITA - Movimentar o chefe para direita");
    mvprintw(lin++, col, "  Seta para ESQUERDA - Movimentar o chefe para esquerda");
    mvprintw(lin++, col, "  SPACE - Interagir com um tripulante");
    mvprintw(lin++, col, "           (para entregar um pedido a ser feito)");
    lin++;
    mvprintw(lin++, col, "  Para entregar o pedido é necessário chegar do lado");
    mvprintw(lin++, col, "  do tripulante, quando o mesmo está disponível, apertar");
    mvprintw(lin++, col, "  space, digitar o número do pedido que você quer que o");
    mvprintw(lin++, col, "  tripulante faça e apertar space novamente para confiramar");
    lin++;

    mvprintw(lin++, col, "Como Jogar:");
    mvprintw(lin++, col, "  - Pedidos aparecem no canto inferior direito.");
    mvprintw(lin++, col, "  - Chegue perto de um tripulante e pressione SPACE, ID do Pedido, SPACE.");
    mvprintw(lin++, col, "  - Os tripulantes preparam e cozinham a comida automaticamente.");
    mvprintw(lin++, col, "  - Quando o prato estiver pronto, a pontuação aumenta.");
    lin++;

    mvprintw(lin++, col, "Informações da tela:");
    mvprintw(lin++, col, "  - Pontuação e tempo são exibidos no topo direito.");
    mvprintw(lin++, col, "  - Bancadas na cor verde = disponíveis.");
    mvprintw(lin++, col, "  - Bancadas na cor magenta = ocupadas.");
    lin++;

    mvprintw(lin++, col, "Dica:");
    mvprintw(lin++, col, "  Administre bem o tempo e a quantidade de pedidos.");
    mvprintw(lin++, col, "  Quanto mais pedidos entregar, maior será sua nota!");
    lin++;

    mvprintw(lin++, col, "Pressione qualquer tecla para voltar ao menu...");

    attroff(COLOR_PAIR(1));
    refresh();
    getch();
}

void menu() {    
    nodelay(stdscr, FALSE);

    desenhar_tela();
    print_logo();

    char op;
    do {
        desenhar_tela();
        print_logo();
        op = getch();
        switch(op) {
            case '1':
                escolher_dificuldade();
                break;

            case '2':
                mostrar_tutorial();
                break;

            case '3':
                printw("Saindo...");
                break;
            
            default:
                break;
        }

    } while (op != '3');
    
    refresh();              // Atualiza a tela com o conteúdo desenhado
}
