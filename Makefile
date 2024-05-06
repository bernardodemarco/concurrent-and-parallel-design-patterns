CC=gcc

FLAGS=-pthread \
      -o

BUILD=./build

SRC=./src

C_FILES=$(SRC)/*.c \
        $(SRC)/sensor/*.c \
        $(SRC)/orchestrator/*.c \
        $(SRC)/data-structures/queue/*.c \
        $(SRC)/data-structures/hashmap/*.c \
        $(SRC)/globals/*.c \
        $(SRC)/utils/*.c \
        $(SRC)/lib/C-Thread-Pool/thpool.c \

app:
	$(CC) $(C_FILES) $(FLAGS) $(BUILD)/app
run:
	$(BUILD)/app
clean:
	rm $(BUILD)/*
