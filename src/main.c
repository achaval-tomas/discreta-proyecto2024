#include "APIG24.h"
#include "API2024Parte2.h"
#include <stdio.h>

void ImprimirGrafo(Grafo g)
{
    printf("\nn: %u, m: %u, delta: %u\n", NumeroDeVertices(g), NumeroDeLados(g), Delta(g));

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

// test ImportarColores y AsignarColor
void ColorearGrafo(Grafo g)
{
    printf("\nColoreando...\n");
    u32 n = NumeroDeVertices(g);
    color* Color = calloc(n, sizeof(color));
    for (u32 i = 0; i < n; ++i)
        Color[i] = n - i;
    ImportarColores(Color, g);
    free(Color);

    // no deberia afectar (K5)
    AsignarColor(99, 10, g);
}

// test ExtraerColores
void ImprimirColores(Grafo g)
{

    u32 n = NumeroDeVertices(g);
    color* Colores = calloc(n, sizeof(color));
    ExtraerColores(g, Colores);

    printf("Colores Utilizados: ");
    for (u32 i = 0; i < n; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%u", Colores[i]);
    }
    printf("\n");

    free(Colores);
}

int main()
{
    Grafo grafo = ConstruirGrafo();

    if (grafo == NULL) {
        fprintf(stderr, "Error al leer el grafo!\n");
        return EXIT_FAILURE;
    }

    ImprimirGrafo(grafo);

    u32 n = NumeroDeVertices(grafo);
    u32* orden = calloc(n, sizeof(u32));
    for (u32 i = 0; i<n; ++i)
        orden[i] = i;
    Greedy(grafo, orden);
    free(orden);
    ImprimirColores(grafo);

    ImprimirGrafo(grafo);

    printf("\n");
    printf("Grado de vértice out of bounds: %u\n", Grado(NumeroDeVertices(grafo), grafo));
    printf("Color de vértice out of bounds: %u\n", Color(NumeroDeVertices(grafo), grafo));

    DestruirGrafo(grafo);

    return 0;
}
