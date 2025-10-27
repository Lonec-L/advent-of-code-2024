

all: 14

13: build13 run13
14: build14 run14

build13:
	gcc -Iinclude src/13.c -o ./build/13.out

run13:
	@echo "day 13:"
	./build/13.out

build14:
	gcc -Iinclude src/14.c -o ./build/14.out

run14:
	@echo "day 14:"
	./build/14.out

clean:
	rm -rf ./build/*.out
