#include "APIG24.h"
#include <assert.h>
#include <stdio.h>

void ConsumirComentarios()
{
    // Hacen falta 2 char-s debido a que scanf() le agrega \0 al final
    char c[2];
    while (scanf("c %*[^\n]%1[\n]", c) == 1)
        ;
}

void AgregarVecino(VerticeSt* vertice, u32 w)
{
    if (vertice->capacidad == vertice->grado) {
        vertice->capacidad = (vertice->capacidad == 0) ? 1 : vertice->capacidad * 2;
        vertice->vecinos = realloc(vertice->vecinos, vertice->capacidad * sizeof(u32));
    }

    vertice->grado += 1;
    vertice->vecinos[vertice->grado - 1] = w;
}

u32 LeerLados(VerticeSt* vertices, u32 n, u32 m)
{
    for (u32 i = 0; i < m; ++i) {
        u32 v, w;
        if (scanf("e %u %u\n", &v, &w) != 2)
            return i;

        assert(v < n && w < n);

        AgregarVecino(&vertices[v], w);
        AgregarVecino(&vertices[w], v);
    }

    return m;
}

u32 CalcularDelta(VerticeSt* vertices, u32 n)
{
    u32 delta = 0;
    for (u32 i = 0; i < n; i++) {
        VerticeSt* v = &vertices[i];
        delta = (v->grado > delta) ? v->grado : delta;
    }
    return delta;
}

Grafo ConstruirGrafo()
{
    ConsumirComentarios();
    u32 n, m;
    if (scanf("p edge %u %u\n", &n, &m) != 2)
        return NULL;

    if (n < 2)
        return NULL;
    if (m < 1)
        return NULL;

    VerticeSt* vertices = calloc(n, sizeof(VerticeSt));

    u32 lados_leidos = LeerLados(vertices, n, m);
    if (lados_leidos < m) {
        free(vertices);
        return NULL;
    }

    Grafo grafo = malloc(sizeof(GrafoSt));
    grafo->vertices = vertices;
    grafo->num_vertices = n;
    grafo->num_lados = m;
    grafo->delta = CalcularDelta(vertices, n);

    return grafo;
}

void DestruirGrafo(Grafo g)
{
    for (u32 v = 0; v < g->num_vertices; v++) {
        free(g->vertices[v].vecinos);
    }

    free(g->vertices);
    free(g);
}

u32 NumeroDeVertices(Grafo g)
{
    return g->num_vertices;
}

u32 NumeroDeLados(Grafo g)
{
    return g->num_lados;
}

u32 Delta(Grafo g)
{
    return g->delta;
}
