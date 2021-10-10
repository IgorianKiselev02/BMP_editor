MKDIR_P = mkdir -p

.PHONY: directories

all: directories obj/main.o obj/bmp.o hw_01
directories: obj
obj:
	${MKDIR_P} obj
obj/bmp.o: src/bmp.c include/bmp.h
	gcc -c -Wall -Wextra -Werror src/bmp.c -I include -o obj/bmp.o
obj/main.o: src/main.c include/bmp.h
	gcc -c -Wall -Wextra -Werror src/main.c -I include -o obj/main.o
hw_01: obj/bmp.o obj/main.o
	gcc obj/bmp.o obj/main.o -o hw_01
clean:
	rm -r obj hw_01