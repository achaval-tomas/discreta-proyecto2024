#include "API2024Parte2.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "Sort.h"

u32 CorrerIteraciones(Grafo g, u32* orden)
{
    u32 ncolores;
    clock_t start, end;

    for (u32 i = 0; i < 50; i++) {
        start = clock();
        if (GulDukat(g, orden)) {
            printf("Error GulDukat\n");
            exit(EXIT_FAILURE);
        }
        end = clock();
        printf("\tGulDukat took: %fs\n", (float)(end - start) / CLOCKS_PER_SEC);

        start = clock();
        ncolores = Greedy(g, orden);
        if (ncolores == UINT32_MAX) {
            printf("Error Greedy\n");
            exit(EXIT_FAILURE);
        }
        end = clock();
        printf("\tGreedy took: %fs\n", (float)(end - start) / CLOCKS_PER_SEC);

        printf("\tCantidad de colores luego de GulDukat: %u\n", ncolores);

        start = clock();
        if (ElimGarak(g, orden)) {
            printf("Error ElimGarak\n");
            exit(EXIT_FAILURE);
        }
        end = clock();
        printf("\tElimGarak took: %fs\n", (float)(end - start) / CLOCKS_PER_SEC);

        start = clock();
        ncolores = Greedy(g, orden);
        if (ncolores == UINT32_MAX) {
            printf("Error Greedy\n");
            exit(EXIT_FAILURE);
        }
        end = clock();
        printf("\tGreedy took: %fs\n", (float)(end - start) / CLOCKS_PER_SEC);

        printf("\tCantidad de colores luego de ElimGarak: %u\n", ncolores);
        fflush(stdout);
    }

    return ncolores;
}

u32 CorrerIteracionesRandom(Grafo g, u32* orden)
{
    u32 ncolores;

    for (u32 i = 0; i < 500; i++) {
        if (rand() % 2 == 0) {
            if (GulDukat(g, orden)) {
                printf("Error GulDukat\n");
                exit(EXIT_FAILURE);
            }

            ncolores = Greedy(g, orden);
            if (ncolores == UINT32_MAX) {
                printf("Error Greedy\n");
                exit(EXIT_FAILURE);
            }

            printf("\tCantidad de colores luego de GulDukat: %u\n", ncolores);
        } else {
            if (ElimGarak(g, orden)) {
                printf("Error ElimGarak\n");
                exit(EXIT_FAILURE);
            }

            ncolores = Greedy(g, orden);
            if (ncolores == UINT32_MAX) {
                printf("Error Greedy\n");
                exit(EXIT_FAILURE);
            }

            printf("\tCantidad de colores luego de ElimGarak: %u\n", ncolores);
        }
        fflush(stdout);
    }

    return ncolores;
}

#define NCASOS 5

int main(void)
{
    srand(time(NULL));

    Grafo g = ConstruirGrafo();

    if (g == NULL) {
        fprintf(stderr, "Error al leer el grafo!\n");
        return EXIT_FAILURE;
    }

    u32 n = NumeroDeVertices(g);
    printf("n: %u\n", n);

    color* colores[NCASOS];
    u32* ordenes[NCASOS];
    u32 cant_colores[NCASOS];
    for (u32 i = 0; i < NCASOS; i++) {
        colores[i] = malloc(n * sizeof(color));
        ordenes[i] = malloc(n * sizeof(u32));
        for (u32 j = 0; j < n; j++)
            ordenes[i][j] = j;
    }

    { // Primer caso, vertices creciente.
        for (u32 i = 0; i < n; i++)
            ordenes[0][i] = i;
    }

    { // Segundo caso, vertices decreciente.
        for (u32 i = 0; i < n; i++)
            ordenes[1][i] = (n - 1) - i;
    }

    { // Tercer caso: primero vertices pares decreciente, luego impares creciente.
        u32 cant_pares = (n + 1) / 2;
        u32 cant_impares = n - cant_pares;
        for (u32 i = 0; i < cant_pares; i++)
            ordenes[2][i] = 2 * ((cant_pares - 1) - i);
        for (u32 i = 0; i < cant_impares; i++)
            ordenes[2][cant_pares + i] = 2 * i + 1;
    }

    { // Cuarto caso: vertices por su grado decreciente.
        u32 delta = Delta(g);

        int res;
        LINEAR_SORT(res, ordenes[3], n, delta, v, d, {
            d = delta - Grado(v, g);
        });
        assert(res == 0);
    }

    { // Quinto caso: orden cocktail (0, n-1, 1, n-2, 2, n-3, ...).
        for (u32 i = 0; i < n; i++) {
            if (i % 2 == 0) {
                ordenes[4][i] = i / 2;
            } else {
                ordenes[4][i] = (n - 1) - (i / 2);
            }
        }
    }

    for (u32 i = 0; i < NCASOS; i++) {
        printf("\n\nCaso %u:\n", i + 1);

        // for (u32 j = 0; j < n; j++)
        //     printf("\t%u", ordenes[i][j]);
        // printf("\n");

        // for (u32 j = 0; j < n; j++)
        //     printf("\t%u", Grado(ordenes[i][j], g));
        // printf("\n");

        // Greedy inicial
        u32 ncolores = Greedy(g, ordenes[i]);
        printf("\tCantidad de colores luego del Greedy inicial: %u\n", ncolores);

        // Iteraciones
        cant_colores[i] = CorrerIteraciones(g, ordenes[i]);
        ExtraerColores(g, colores[i]);
        printf("\tCantidad de colores luego de todas las iteraciones: %u\n", cant_colores[i]);
    }

    u32 mejor_caso = 0;
    for (u32 i = 1; i < NCASOS; i++) {
        if (cant_colores[i] < cant_colores[mejor_caso]) {
            mejor_caso = i;
        }
    }

    printf("\n\nEl mejor caso fue %u:\n", mejor_caso + 1);
    ImportarColores(colores[mejor_caso], g);
    u32 ncolores = CorrerIteracionesRandom(g, ordenes[mejor_caso]);
    printf("\tCantidad de colores luego de todas las iteraciones: %u\n", ncolores);

    for (u32 i = 0; i < NCASOS; i++) {
        free(colores[i]);
        free(ordenes[i]);
    }
    DestruirGrafo(g);
    printf("\n");
    return EXIT_SUCCESS;
}
