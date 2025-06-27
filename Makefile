# Nome do executável
EXEC = overcooked

# Diretórios
SRC_DIR = ./src
OBJ_DIR = ./build
INC_DIR = ./include

# Arquivos-fonte
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto (na pasta build/)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Compilador e flags
CC = gcc
CFLAGS = -Wall -g -I$(INC_DIR)

# Regra padrão
all: $(EXEC)

# Como gerar o executável
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lncurses

# Como compilar cada .c em .o (na pasta build)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Garante que a pasta build/ existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(EXEC)
