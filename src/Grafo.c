#include "APIG24.h"
#include <stdio.h>
#include <assert.h>

#define ERROR(...)                    \
    {                                 \
        fprintf(stderr, "ERROR: ");   \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
        exit(EXIT_FAILURE);           \
    }

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
    if (scanf("p edge %u %u\n", &n, &m) != 2)
        ERROR("no se pudo leer el formato 'p edge %%u %%u'");

    if (n < 2)
        ERROR("se especificaron %u vertices pero la cantidad minima es 2", n);
    if (m < 1)
        ERROR("se especificaron %u lados pero la cantidad minima es 1", m);

    VerticeSt* vertices = calloc(n, sizeof(VerticeSt));

    u32 lados_leidos = LeerLados(vertices, n, m);
    if (lados_leidos < m)
        ERROR("se esperaban %u lados pero sólo se leyeron %u.\n", m, lados_leidos);

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
