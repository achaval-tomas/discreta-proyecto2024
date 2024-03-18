#include <stdio.h>
#include "APIG24.h"

void ImprimirGrafo(Grafo g)
{
    printf("n: %u, m: %u, delta: %u\n", NumeroDeVertices(g), NumeroDeLados(g), Delta(g));

    for (u32 i = 0; i < g->num_vertices; i++) {
        VerticeSt* v = &g->vertices[i];
        printf("[%u, color: %u, grado %u]: ", i, Color(i, g), Grado(i, g));
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

    printf("Grado de vértice out of bounds: %u\n", Grado(NumeroDeVertices(grafo), grafo));
    printf("Color de vértice out of bounds: %u\n", Color(NumeroDeVertices(grafo), grafo));

    DestruirGrafo(grafo);

    return 0;
}
