#include "APIG24.h"
#include <stdio.h>
#include <assert.h>

void ConsumirComentarios()
{
    // Hacen falta 2 char-s debido a que scanf() le agrega \0 al final
    char c[2];
    while (scanf("c %*[^\n]%1[\n]", c) == 1)
        ;
}

void AgregarVecino(VerticeSt* vertice, u32 w)
{
    vertice->grado += 1;
    if (vertice->grado == 1) {
        vertice->vecinos = malloc(1 * sizeof(u32));
    } else {
        // TODO: multiplicar capacidad del arreglo por una constante
        vertice->vecinos = realloc(vertice->vecinos, vertice->grado * sizeof(u32));
    }

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
    if (scanf("p edge %u %u\n", &n, &m) != 2) {
        // TODO
    }
    assert(n > 1 && m > 0);

    VerticeSt* vertices = calloc(n, sizeof(VerticeSt));
    u32 lados_leidos = LeerLados(vertices, n, m);

    if (lados_leidos < m) {
        fprintf(stderr, "Se esperaron %u lados pero sólo se leyeron %u.\n", m, lados_leidos);
        // TODO: fix mem leak
        return NULL;
    }

    Grafo grafo = malloc(sizeof(GrafoSt));
    grafo->vertices = vertices;
    grafo->num_vertices = n;
    grafo->num_lados = m;
    grafo->delta = CalcularDelta(vertices, n);

    return grafo;
}

void DestruirGrafo(Grafo G)
{
    // TODO
}
