FILES=main.c Grafo.c

COMMON_ARGS=-Wall -Werror -std=c99
DEBUG_ARGS=-g
RELEASE_ARGS=-O3

SOURCES=$(patsubst %, src/%, $(FILES))

EXECUTABLE_NAME=main
BUILD_DIR=build

DEBUG_DIR=${BUILD_DIR}/debug
RELEASE_DIR=${BUILD_DIR}/release

DEBUG_EXECUTABLE=${DEBUG_DIR}/${EXECUTABLE_NAME}
RELEASE_EXECUTABLE=${RELEASE_DIR}/${EXECUTABLE_NAME}

${DEBUG_EXECUTABLE}: ${SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${DEBUG_ARGS} -o ${DEBUG_EXECUTABLE} ${SOURCES}

${RELEASE_EXECUTABLE}: ${SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${RELEASE_ARGS} -o ${RELEASE_EXECUTABLE} ${SOURCES}

build: ${DEBUG_EXECUTABLE}

run: ${DEBUG_EXECUTABLE}
	./${DEBUG_EXECUTABLE} < K5.txt

release: ${RELEASE_EXECUTABLE}
	./${RELEASE_EXECUTABLE} < K5.txt

clean:
	rm -rf ./${BUILD_DIR}