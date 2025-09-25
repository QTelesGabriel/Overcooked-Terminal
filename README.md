# Jogo de Cozinha Concorrente (Overcooked - Terminal Edition)

Bem-vindo ao Overcooked - Terminal Edition\! Este é um jogo de cozinha desenvolvido em C que simula o ambiente caótico e divertido de uma cozinha, mas com um toque de programação concorrente. Controle o Chefe, gerencie seus Tripulantes e prepare o máximo de pedidos possível antes que o tempo acabe, tudo diretamente no seu terminal\!

-----

## 1\. Introdução

Este projeto implementa um jogo de cozinha utilizando **programação concorrente em C** com a biblioteca **`pthread`** para gerenciar as threads e **`ncurses`** para a interface de usuário no terminal. O foco está em simular tarefas paralelas (geração de pedidos, movimentação de personagens, preparo e cozimento de alimentos, atualização da tela) e na coordenação dessas tarefas por meio de mecanismos de sincronização eficientes.

-----

## 2\. Arquitetura de Threads

O jogo é estruturado em um modelo **multi-threaded**, onde cada entidade ou processo crucial é gerenciado por sua própria thread, garantindo a execução simultânea das operações. Conheça as principais threads:

  * **Thread do Mural de Pedidos (`thread_mural_de_pedidos`)**: Responsável por gerar novos pedidos em intervalos regulares, adicionando-os a uma **lista global de pedidos**.
  * **Thread do Chefe de Cozinha (`thread_chefe_da_cozinha`)**: Controla a **movimentação do personagem principal** (o Chefe) e sua interação com o ambiente. Esta thread captura a entrada do usuário (`getch()`) e atribui pedidos aos tripulantes.
  * **Threads dos Tripulantes (`thread_tripulantes`)**: Existem **N threads tripulantes** (quantidade configurável pela dificuldade do jogo). Cada tripulante é responsável por processar um pedido que lhe é **atribuído pelo Chefe**. O processo inclui:
      * Definir um destino para uma **bancada de preparo** (`definir_destino`).
      * Mover-se até a bancada (`mover_tripulante`).
      * Simular o **tempo de preparo** do alimento.
      * Definir um destino para uma **bancada de cozinha**.
      * Mover-se até a bancada.
      * Simular o **tempo de cozimento**.
      * Remover o pedido da lista e **incrementar a pontuação**.
      * Retornar a uma posição ociosa (`"fim"`).
        Os tripulantes ficam em um estado de espera até que um pedido seja atribuído.
  * **Thread de Exibição de Informações (`thread_exibir_informacoes`)**: Atualiza constantemente a interface do `ncurses`, desenhando o cenário do jogo, os personagens, os pedidos ativos e as informações vitais (tempo, pontuação).
  * **Thread do Temporizador (`thread_temporizador`)**: Gerencia o **tempo total de jogo**, decrementando um contador. Quando o tempo se esgota, ele sinaliza o fim do jogo.

-----

## 3\. Seções Críticas e Sincronização

A concorrência no jogo exige um controle rigoroso para evitar **condições de corrida** (race conditions) e garantir a **integridade dos dados** e a **coordenação das ações**. Para isso, utilizamos **mutexes** (travas) e **variáveis de condição**.

### 3.1. Mutexes (Travas)

Os seguintes mutexes são utilizados para proteger o acesso a **seções críticas**, garantindo que apenas uma thread possa modificar um recurso compartilhado por vez:

  * `lista_lock`: Protege o acesso à estrutura global `ListaDePedidos` (`lista`). Isso evita que múltiplas threads adicionem ou removam pedidos simultaneamente, garantindo a consistência da lista.
  * `ncurses_lock`: Essencial para proteger **todas as operações de I/O da biblioteca `ncurses`** (e.g., `mvprintw`, `refresh`, `getch`). O `ncurses` não é thread-safe por padrão; este mutex previne corrupção visual e comportamentos inesperados na tela.
  * `lock_jogo`: Protege a variável global `jogo_esta_valendo`. Garante que o estado do jogo (ativo/finalizado) seja alterado e lido de forma atômica por diversas threads.
  * `lock_tempo`: Protege a variável `tempo_de_jogo`. Garante que o contador de tempo seja decrementado e lido corretamente.
  * `id_pedido_lock`: Protege a variável global `id_pedido_que_sera_feito`. Garante que o ID do pedido que o Chefe deseja atribuir seja lido de forma consistente.
  * `preparo_lock` e `cozinha_lock`: Protegem as estruturas `bancada_preparo` e `bancada_cozinha`. Permitem que as threads dos tripulantes **alquem e liberem bancadas** de forma segura, evitando que duas threads ocupem a mesma bancada simultaneamente.
  * `tripulante[i].lock`: Cada thread de tripulante possui seu próprio mutex individual. Ele protege os atributos específicos do tripulante (e.g., `destino_x`, `destino_y`, `pedido_id`, `tem_pedido`), garantindo que o Chefe possa atribuir um pedido sem interferir nas operações internas do tripulante.

### 3.2. Variáveis de Condição

  * `tripulante[i].cond`: Associada a `tripulante[i].lock`. Cada tripulante usa esta variável de condição para entrar em `pthread_cond_wait()` enquanto está ocioso (`t->tem_pedido == 0`). Quando o Chefe atribui um pedido, ele usa `pthread_cond_signal()` para "acordar" o tripulante específico. Isso evita o ***busy-waiting*** (consumo excessivo de CPU) por parte das threads ociosas, tornando o sistema mais eficiente.

### 3.3. Fluxo de Sincronização

1.  **Menu e Inicialização**: A função `menu()` gerencia a tela inicial, tutorial e seleção de dificuldade. Ao iniciar o jogo (`escolher_dificuldade` chama `jogar()`), `jogar()` configura o `ncurses` para modo não-bloqueante (`nodelay(stdscr, TRUE)`) e inicia todas as threads do jogo.
2.  **Geração de Pedidos**: A `thread_mural_de_pedidos` periodicamente adquire `lista_lock` para adicionar novos pedidos.
3.  **Movimentação e Interação do Chefe**: A `thread_chefe_da_cozinha` captura o input do usuário (`getch()`), protegida por `ncurses_lock`. Quando o Chefe interage para atribuir um pedido:
      * Ele adquire `tripulante[i].lock` para o tripulante alvo.
      * Atualiza o `t->pedido_id` do tripulante (o `id_pedido_que_sera_feito` é protegido por `id_pedido_lock`).
      * Define `t->tem_pedido = 1`.
      * Envia um `pthread_cond_signal()` para o tripulante e libera o mutex.
4.  **Processamento dos Tripulantes**:
      * Uma `thread_tripulantes` espera em `pthread_cond_wait()` enquanto `t->tem_pedido` é `0`.
      * Ao ser sinalizada, ela acorda, adquire `t->lock`, verifica o pedido e libera o mutex.
      * Ela adquire `preparo_lock`, usa `definir_destino` para encontrar uma bancada de preparo livre, e a marca como ocupada.
      * O tripulante se move (`mover_tripulante`) e simula o tempo de preparo com `usleep`.
      * A bancada de preparo é liberada.
      * Um processo similar ocorre para a bancada de cozinha, utilizando `cozinha_lock`.
      * Finalmente, `lista_lock` é adquirido para `remover_pedido`, e a `pontuacao` é incrementada.
      * O tripulante retorna a uma posição "ociosa" e define `t->tem_pedido = 0`, voltando a esperar por um novo pedido.
5.  **Atualização da Interface**: A `thread_exibir_informacoes` constantemente adquire `ncurses_lock` para realizar todas as operações de desenho (`desenhar_tela`, `mostrar_pedidos`, `mostrar_chefe`, etc.) e então libera o mutex. Ela também lê `tempo_de_jogo` (protegido por `lock_tempo`).
6.  **Fim do Jogo**: Quando a `thread_temporizador` zera o `tempo_de_jogo` ou o Chefe pressiona 'P'/'p', `lock_jogo` é adquirido e `jogo_esta_valendo` é definido como `0`. Um sinal (`pthread_cond_signal`) é enviado para cada thread de tripulante para garantir que saiam de seus estados de espera e possam encerrar.
7.  **Finalização**: A função `jogar()` espera o término de todas as threads (`pthread_join`). A ordem de `pthread_join` é crucial: a `thread_exibir_informacoes` é a última a ser "joinada" porque é ela quem chama a `tela_final()` (que contém um `getch()` final bloqueante), garantindo que a tela seja exibida e o input seja capturado somente após todas as outras threads terem sido encerradas. Após isso, o jogo retorna ao `menu()`.

-----

## 4\. Guia de Utilização do Jogo

Este guia descreve como instalar e executar o jogo em um ambiente Linux (Ubuntu 22.04 ou similar), utilizando o `Makefile` fornecido.

### 4.1. Pré-requisitos

Antes de compilar e executar o jogo, certifique-se de ter as seguintes ferramentas e bibliotecas instaladas no seu sistema. Abra um terminal e execute os comandos abaixo para instalá-los:

```bash
sudo apt update
sudo apt install build-essential libncurses-dev
```

  * **`build-essential`**: Inclui o **GCC (GNU Compiler Collection)**, o compilador C necessário, e o **Make**, a ferramenta de automação de compilação.
  * **`libncurses-dev`**: Biblioteca de desenvolvimento ncurses, essencial para a interface gráfica do jogo no terminal.

### 4.2. Compilação do Jogo

Para compilar o jogo, siga os passos abaixo:

1.  **Estrutura de Diretórios**: Verifique se o seu projeto está organizado da seguinte forma:

    ```
    .
    ├── Makefile
    ├── src/
    │   ├── main.c
    │   ├── (outros_arquivos).c
    │   └── ...
    └── include/
        ├── (arquivos_de_cabeçalho).h
        └── ...
    ```

    O `Makefile` espera que os arquivos `.c` estejam dentro do diretório `src/` e os arquivos `.h` dentro de `include/`. Os arquivos objeto (`.o`) e o executável serão gerados em um novo diretório `build/`.

2.  **Navegar até o Diretório do Projeto**: Abra um terminal e navegue até o diretório raiz do seu projeto, onde o `Makefile` está localizado:

    ```bash
    cd /caminho/para/seu/projeto/
    ```

    (Substitua `/caminho/para/seu/projeto/` pelo caminho real do seu projeto.)

3.  **Executar o Make**: Com o `Makefile` no diretório atual, execute o comando `make`:

    ```bash
    make
    ```

    Este comando fará o seguinte:

      * Criará o diretório `build/` se ele ainda não existir.
      * Compilará cada arquivo `.c` do diretório `src/` em um arquivo objeto (`.o`) correspondente, salvando-o dentro de `build/`.
      * Linkará todos os arquivos objeto com a biblioteca `ncurses` para criar o executável final, `overcooked`, no diretório raiz do projeto.

    Se a compilação for bem-sucedida, você verá o executável `overcooked` aparecer no mesmo diretório do `Makefile`.

### 4.3. Execução do Jogo

Após a compilação, você pode executar o jogo diretamente do terminal:

```bash
./overcooked
```

### 4.4. Controles do Jogo

Ao iniciar o jogo, você será levado a um menu principal.

  * **Menu Principal**:

      * Pressione **'1'** para iniciar o jogo.
      * Pressione **'2'** para ver o tutorial (instruções detalhadas sobre os controles e o *gameplay*).
      * Pressione **'3'** para sair do jogo.

  * **Seleção de Dificuldade**: Após escolher iniciar o jogo, selecione a dificuldade desejada:

      * **'1'**: Fácil (3 tripulantes, 120s de jogo, 6s para novos pedidos)
      * **'2'**: Médio (4 tripulantes, 100s de jogo, 4s para novos pedidos)
      * **'3'**: Difícil (5 tripulantes, 80s de jogo, 3s para novos pedidos)

  * **Dentro do Jogo**:

      * **Setas (UP, DOWN, LEFT, RIGHT)**: Movimentam o Chefe de Cozinha pelo cenário.
      * **Barra de Espaço (`     `)**: Inicia o processo de atribuir um pedido a um tripulante. Para isso:
        1.  Posicione o Chefe próximo a um **tripulante disponível**.
        2.  Pressione **ESPAÇO**.
        3.  Digite o **ID do pedido** que deseja que o tripulante prepare (IDs são exibidos no mural de pedidos).
        4.  Pressione **ESPAÇO** novamente para confirmar a atribuição.
      * **'p' ou 'P'**: Encerra o jogo imediatamente, exibindo a tela final de pontuação. Pressionar qualquer tecla na tela final o retornará ao menu principal.

### 4.5. Informações Visuais na Tela

  * **Bancadas Verdes**: Indicam bancadas de preparo ou cozinha **disponíveis**.
  * **Bancadas Magenta**: Indicam bancadas de preparo ou cozinha **ocupadas** por um tripulante.
  * **Pontuação e Tempo**: Exibidos no canto superior direito da tela de jogo.

### 4.6. Limpeza (Opcional)

Para remover os arquivos compilados (executáveis e arquivos objeto) do seu diretório, execute o comando `make clean` no diretório do projeto:

```bash
make clean
```

Isso removerá o executável `overcooked` e todo o conteúdo do diretório `build/`.

-----

### 5. Imagens
<img width="944" height="917" alt="image" src="https://github.com/user-attachments/assets/2085f95a-4d73-4151-a0d6-2f3ccf389015" />
<img width="937" height="911" alt="image" src="https://github.com/user-attachments/assets/2cb10478-2510-448a-9a28-810a2a085708" />
<img width="938" height="912" alt="image" src="https://github.com/user-attachments/assets/31ddc7d5-7105-4b3d-885c-73d5f4689200" />


# Autor

## Gabriel Queiroz Teles
