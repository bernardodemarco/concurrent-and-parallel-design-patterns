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
        $(SRC)/utils/*.c \
        $(SRC)/lib/C-Thread-Pool/thpool.c \

compile:
	$(CC) $(C_FILES) $(FLAGS) $(BUILD)/app
run:
	$(BUILD)/app
app:
	$(CC) $(C_FILES) $(FLAGS) $(BUILD)/app && $(BUILD)/app
clean:
	rm $(BUILD)/*
