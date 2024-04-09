#include "Util.h"
#include "Math.h"
#include <assert.h>

u32 CalcularMaxColor(Grafo G)
{
    u32 max_color = 0;
    for (u32 v = 0; v < NumeroDeVertices(G); v++) {
        assert(Color(v, G) != 0); // TODO
        max_color = MAX(max_color, Color(v, G));
    }
    return max_color;
}

// Chequear que el orden dado es un orden válido.
char ChequearOrden(u32* Orden, u32 n)
{
    char* found_flags = calloc(n, sizeof(char));

    int is_valid = 1;
    for (u32 i = 0; i < n; ++i) {
        u32 v = Orden[i];

        if (v >= n || found_flags[v]) {
            is_valid = 0;
            break;
        }

        found_flags[v] = 1;
    }

    free(found_flags);
    return is_valid;
}

static int ChequearOrdenPorBloques(Grafo G, u32* Orden)
{
    u32 r = CalcularMaxColor(G);

    char* flags_bloques = calloc(r, sizeof(char));

    u32 i = 0;
    while (i < NumeroDeVertices(G)) {
        u32 v = Orden[i];
        u32 c = Color(v, G);

        if (flags_bloques[c - 1]) {
            printf("ERROR: reordenamiento inválido\n");
            return 0;
        }
        flags_bloques[c - 1] = 1;

        // Avanzar i hasta encontrar un vértice con otro color, o hasta llegar al último vértice.
        do {
            i += 1;
        } while (v < NumeroDeVertices(G) && Color(Orden[i], G) == c);
    }

    free(flags_bloques);
    return 1;
}

char ChequearReordenamientoVIT(Grafo G, u32* Orden)
{
    if (!ChequearOrden(Orden, NumeroDeVertices(G))) {
        return 0;
    }

    if (!ChequearOrdenPorBloques(G, Orden)) {
        return 0;
    }

    return 1;
}