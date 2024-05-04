#include "APIG24.h"
#include "Math.h"
#include "Util.h"
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

u32 Grado(u32 i, Grafo g)
{
    if (i >= NumeroDeVertices(g))
        return 0;

    return g->vertices[i].grado;
}

color Color(u32 i, Grafo g)
{
    if (i >= NumeroDeVertices(g))
        return -1; // 2³² - 1

    return g->vertices[i].color;
}

u32 Vecino(u32 j, u32 i, Grafo g)
{
    if (i >= NumeroDeVertices(g) || j >= Grado(i, g))
        return -1; // 2³² - 1

    return g->vertices[i].vecinos[j];
}


// Colorear todos los vertices con 0.
static char ResetearColores(Grafo G)
{
    u32* colores = calloc(NumeroDeVertices(G), sizeof(u32));
    if (colores == NULL)
        return 1;
    ImportarColores(colores, G);
    free(colores);
    return 0;
}

/*
Asume que el color 0 significa que el vértice no está pintado
y que flags tiene espacio suficiente para todos los colores (delta+2).
También se asume que flags[c] != v + 1 para todo c.
Devuelve el mínimo color disponible para el vertice v.
*/
static u32 ProximoColor(Grafo G, u32 v, u32* flags)
{
    u32 grado = Grado(v, G);

    for (u32 i = 0; i < grado; ++i) {
        u32 vecino = Vecino(i, v, G);
        u32 color_vecino = Color(vecino, G);

        // Nos ahorramos el siguiente if:
        // if (color_vecino != 0)
        //     flags[color_vecino] = v + 1;
        // Esto resulta en una buena mejora en algunos grafos como Harkon,
        // pero también reduce el rendimiento en otros grafos como 8suPGJ.
        flags[color_vecino] = v + 1;
    }

    color min_color = 1;
    // SAFE: debe ser que flags[c] != v + 1 para algún c=1, ..., Delta + 1
    while (flags[min_color] == v + 1)
        ++min_color;

    assert(min_color <= Delta(G) + 1);

    return min_color;
}

// Se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);

    // si Orden no es una biyección, retornamos error
    if (!ChequearOrden(Orden, n))
        return -1;

    if (ResetearColores(G))
        return -1;

    // flags será utilizado por ProximoColor para buscar.
    // Se pone espacio para Delta(G) + 2 colores. Explicación: Greedy usará como mucho Delta + 1 colores,
    // pues el vértice con más vecinos tiene Delta vecinos, y si cada vecino tiene un color distinto,
    // los vecinos tendrán en total Delta colores distintos. Luego, todo vértice, independientemente de su grado, se coloreará
    // con algún color en { 1, ..., Delta + 1 }, y por lo tanto, (a priori) esos son todos los colores posibles.
    // Ahora bien, nosotros hacemos una optimización para ahorrar un if la cual consiste en tratar al color 0 (asignado
    // cuando todavía no fue coloreado) como cualquier otro color, y luego al buscar el mínimo color simplemente
    // ignorarlo. Por lo tanto se requieren Delta + 2 lugares, pues los colores posibles son { 0, ..., Delta + 1 }.
    u32* flags = calloc(Delta(G) + 2, sizeof(u32));
    if (flags == NULL)
        return -1;

    u32 num_colores = 0;
    for (u32 i = 0; i < n; i++) {
        u32 v = Orden[i];
        u32 color_v = ProximoColor(G, v, flags);
        AsignarColor(color_v, v, G);
        num_colores = MAX(color_v, num_colores);
    }

    free(flags);
    return num_colores;
}
