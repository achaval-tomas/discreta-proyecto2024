LIB_FILES=Grafo.c Vertice.c Coloreo.c Greedy.c Orden.c Sort.c Sort.h ElimGarak.c Util.h Util.c
MAIN_FILES=main.c test_sort.c test_elim_garak.c test_gul_dukat.c

COMMON_ARGS=-Wall -Wextra -Werror -std=c99
DEBUG_ARGS=-g
SANITIZE_ARGS=-fsanitize=address,undefined
RELEASE_ARGS=-O3
VALGRIND_OPTIONS=--leak-check=full --show-leak-kinds=all --track-origins=yes --quiet

LIB_SOURCES=$(patsubst %,src/%, $(LIB_FILES))
MAIN_SOURCES=$(patsubst %,src/%, $(MAIN_FILES))

MAIN_NAMES=$(basename $(MAIN_FILES))

BUILD_DIR=build
DEBUG_DIR=${BUILD_DIR}/debug
SANITIZE_DIR=${BUILD_DIR}/sanitize
RELEASE_DIR=${BUILD_DIR}/release

DEBUG_EXECUTABLES=$(addprefix $(DEBUG_DIR)/, $(MAIN_NAMES))
SANITIZE_EXECUTABLES=$(addprefix $(SANITIZE_DIR)/, $(MAIN_NAMES))
RELEASE_EXECUTABLES=$(addprefix $(RELEASE_DIR)/, $(MAIN_NAMES))

TEST_SORT_EXECUTABLE=${DEBUG_DIR}/${TEST_SORT_EXECUTABLE_NAME}

all: ${DEBUG_EXECUTABLES}

clean:
	rm -rf ./${BUILD_DIR}

${DEBUG_EXECUTABLES}: $(DEBUG_DIR)/%: src/%.c ${LIB_SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${DEBUG_ARGS} -o $@ $< ${LIB_SOURCES}

${SANITIZE_EXECUTABLES}: $(SANITIZE_DIR)/%: src/%.c ${LIB_SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${SANITIZE_ARGS} -o $@ $< ${LIB_SOURCES}

${RELEASE_EXECUTABLES}: $(RELEASE_DIR)/%: src/%.c ${LIB_SOURCES}
	@mkdir -p $(@D)
	gcc ${COMMON_ARGS} ${RELEASE_ARGS} -o $@ $< ${LIB_SOURCES}

DEF_DEBUG_EXECUTABLE=$(word 1,$(DEBUG_EXECUTABLES))
DEF_SANITIZE_EXECUTABLE=$(word 1,$(SANITIZE_EXECUTABLES))
DEF_RELEASE_EXECUTABLE=$(word 1,$(RELEASE_EXECUTABLES))

# Se puede cambiar el archivo usando 'make run i=grafo.txt'
i=K5.txt

build: ${DEF_DEBUG_EXECUTABLE}

run: $(DEF_DEBUG_EXECUTABLE)
	valgrind ${VALGRIND_OPTIONS} ./$(DEF_DEBUG_EXECUTABLE) < ${i}

sanitize: ${DEF_SANITIZE_EXECUTABLE}
	./${DEF_SANITIZE_EXECUTABLE} < ${i}

release: ${DEF_RELEASE_EXECUTABLE}
	./${DEF_RELEASE_EXECUTABLE} < ${i}

RUN_TARGETS=$(addprefix run_,$(MAIN_NAMES))
SANITIZE_TARGETS=$(addprefix sanitize_,$(MAIN_NAMES))
RELEASE_TARGETS=$(addprefix release_,$(MAIN_NAMES))

${RUN_TARGETS}: run_%: $(DEBUG_DIR)/%
	valgrind ${VALGRIND_OPTIONS} ./$< < ${i}

${SANITIZE_TARGETS}: sanitize_%: $(SANITIZE_DIR)/%
	./$< < ${i}

${RELEASE_TARGETS}: release_%: $(RELEASE_DIR)/%
	./$< < ${i}
