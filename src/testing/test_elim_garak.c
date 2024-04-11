#include "API2024Parte2.h"
#include "Util.h"
#include <stdio.h>

int main(void)
{
    Grafo grafo = ConstruirGrafo();

    if (grafo == NULL) {
        fprintf(stderr, "Error al leer el grafo!\n");
        return EXIT_FAILURE;
    }

    // ImprimirGrafo(grafo);

    u32 n = NumeroDeVertices(grafo);
    u32* orden = calloc(n, sizeof(u32));
    for (u32 i = 0; i < n; ++i) {
        orden[i] = i;
    }

    u32 cols = Greedy(grafo, orden);
    printf("\nColores Utilizados: %u\n", cols);

    printf("Orden: ");
    for (u32 i = 0; i < n; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%u", orden[i]);
    }
    printf("\n");

    printf("Color: ");
    for (u32 i = 0; i < n; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%u", Color(orden[i], grafo));
    }
    printf("\n");


    if (ElimGarak(grafo, orden)) {
        printf("Algo salió mal.\n");
        free(orden);
        DestruirGrafo(grafo);
        return EXIT_FAILURE;
    }

    printf("Orden: ");
    for (u32 i = 0; i < n; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%u", orden[i]);
    }
    printf("\n");

    printf("Color: ");
    for (u32 i = 0; i < n; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%u", Color(orden[i], grafo));
    }
    printf("\n");


    if (!ChequearReordenamientoVIT(grafo, orden)) {
        printf("No es un orden VIT!!!!\n");
        free(orden);
        DestruirGrafo(grafo);
        return EXIT_FAILURE;
    }

    printf("\nColores Utilizados: %u\n", cols);

    free(orden);
    DestruirGrafo(grafo);

    return 0;
}
