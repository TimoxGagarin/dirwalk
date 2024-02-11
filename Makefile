CC = gcc
CFLAGS = -std=c11 -g2 -ggdb -I./headers -pedantic -W -Wall -Wextra

.SUFFIXES:
.SUFFIXES: .c .o

DEBUG = ./build/debug
RELEASE = ./build/release

OUT_DIR = $(DEBUG)
MSG_INFO = "Запуск производится в режиме debug..."
vpath %.c src
vpath %.h src
vpath %.o build/debug

ifeq ($(MODE), release)
	CFLAGS = -std=c11 -I./headers -pedantic -W -Wall -Wextra -Werror
	OUT_DIR = $(RELEASE)
	MSG_INFO = "Запуск производится в режиме release"
	vpath %.o build/release
endif

objects = $(OUT_DIR)/main.o $(OUT_DIR)/utils.o $(OUT_DIR)/item_t_funcs.o
prog = $(OUT_DIR)/dirwalk
options = /home/timoxgagarin/.minecraft/assets -s

run: all
	@echo $(MSG_INFO)
	@./$(prog) $(options)

all: $(prog)

$(prog) : $(objects)
	@$(CC) $(CFLAGS) $(objects) -o $@
	
$(OUT_DIR)/%.o : %.c
	@$(CC) -c $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/* test