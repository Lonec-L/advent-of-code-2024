all: 13 14 15

13: build13 run13
14: build14 run14
15: build15 run15
16: build16 run16

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

build15:
	gcc -Iinclude src/15.c -o ./build/15.out

run15:
	@echo "day 15:"
	./build/15.out

build16:
	gcc -Iinclude src/16.c -o ./build/16.out

run16:
	@echo "day 16:"
	./build/16.out



clean:
	rm -rf ./build/*.out
