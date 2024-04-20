build:
	gcc ./src/main.c -Wall -pthread -o ./build/main
run:
	./build/main
clean:
	rm ./build/*