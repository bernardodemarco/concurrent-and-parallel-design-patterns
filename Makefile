compile:
	gcc ./src/main.c -Wall -pthread -o ./build/main
run:
	./build/main
build-and-run:
	gcc ./src/main.c -Wall -pthread -o ./build/main && ./build/main
clean:
	rm ./build/*