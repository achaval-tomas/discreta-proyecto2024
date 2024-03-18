COMMON_ARGS=-Wall -Werror -std=c99
FILES=main.c Grafo.c

main: ${FILES}
	gcc ${COMMON_ARGS} -g -o main ${FILES}

build: main

main_rel: ${FILES}
	gcc ${COMMON_ARGS} -O3 -o main ${FILES}

run: main
	./main < K5.txt

release: main_rel
	./main
