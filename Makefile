CC=gcc

FLAGS=-pthread \
      -o

BUILD=./build

SRC=./src

C_FILES=$(SRC)/*.c \
        $(SRC)/sensor/*.c \
        $(SRC)/producer-consumer/*.c \
        $(SRC)/utils/*.c \

compile:
	$(CC) $(C_FILES) $(FLAGS) $(BUILD)/app
run:
	$(BUILD)/app
app:
	$(CC) $(C_FILES) $(FLAGS) $(BUILD)/app && $(BUILD)/app
clean:
	rm $(BUILD)/*
