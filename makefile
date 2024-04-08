FILES=main.c Grafo.c Vertice.c Coloreo.c Greedy.c Orden.c Sort.c Sort.h
TEST_SORT_FILES=test_sort.c Sort.c Sort.h

COMMON_ARGS=-Wall -Wextra -Werror -std=c99
DEBUG_ARGS=-g
SANITIZE_ARGS=-fsanitize=address,undefined
RELEASE_ARGS=-O3
VALGRIND_OPTIONS=--leak-check=full --show-leak-kinds=all --track-origins=yes --quiet

SOURCES=$(patsubst %, src/%, $(FILES))
TEST_SORT_SOURCES=$(patsubst %, src/%, $(TEST_SORT_FILES))

EXECUTABLE_NAME=main
TEST_SORT_EXECUTABLE_NAME=test_sort
BUILD_DIR=build

DEBUG_DIR=${BUILD_DIR}/debug
SANITIZE_DIR=${BUILD_DIR}/sanitize
RELEASE_DIR=${BUILD_DIR}/release

DEBUG_EXECUTABLE=${DEBUG_DIR}/${EXECUTABLE_NAME}
SANITIZE_EXECUTABLE=${SANITIZE_DIR}/${EXECUTABLE_NAME}
RELEASE_EXECUTABLE=${RELEASE_DIR}/${EXECUTABLE_NAME}

TEST_SORT_EXECUTABLE=${DEBUG_DIR}/${TEST_SORT_EXECUTABLE_NAME}

${DEBUG_EXECUTABLE}: ${SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${DEBUG_ARGS} -o ${DEBUG_EXECUTABLE} ${SOURCES}

${SANITIZE_EXECUTABLE}: ${SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${DEBUG_ARGS} ${SANITIZE_ARGS} -o ${SANITIZE_EXECUTABLE} ${SOURCES}

${RELEASE_EXECUTABLE}: ${SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${RELEASE_ARGS} -o ${RELEASE_EXECUTABLE} ${SOURCES}

${TEST_SORT_EXECUTABLE}: ${TEST_SORT_SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${DEBUG_ARGS} -o ${TEST_SORT_EXECUTABLE} ${TEST_SORT_SOURCES}

build: ${DEBUG_EXECUTABLE}

# Se puede cambiar el archivo usando 'make run i=grafo.txt'
i=K5.txt

run: ${DEBUG_EXECUTABLE}
	valgrind ${VALGRIND_OPTIONS} ./${DEBUG_EXECUTABLE} < ${i}

sanitize: ${SANITIZE_EXECUTABLE}
	./${SANITIZE_EXECUTABLE} < ${i}

release: ${RELEASE_EXECUTABLE}
	./${RELEASE_EXECUTABLE} < ${i}

clean:
	rm -rf ./${BUILD_DIR}

test_sort: ${TEST_SORT_EXECUTABLE}
	valgrind ${VALGRIND_OPTIONS} ./${TEST_SORT_EXECUTABLE} < ${i}