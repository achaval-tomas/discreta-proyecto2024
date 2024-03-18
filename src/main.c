#include <stdio.h>
#include "APIG24.h"

void ImprimirGrafo(Grafo g)
{
    printf("n: %u, m: %u, delta: %u\n", g->num_vertices, g->num_lados, g->delta);

    for (u32 i = 0; i < g->num_vertices; i++) {
        VerticeSt* v = &g->vertices[i];
        printf("[%u]: ", i);
        for (u32 j = 0; j < v->grado; j++) {
            if (j != 0) {
                printf(", ");
            }
            printf("%u", v->vecinos[j]);
        }
        printf("\n");
    }
}

int main()
{
    Grafo grafo = ConstruirGrafo();

    ImprimirGrafo(grafo);

    DestruirGrafo(grafo);

    return 0;
}