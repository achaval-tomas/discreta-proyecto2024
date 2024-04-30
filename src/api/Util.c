#include "Util.h"
#include "APIG24.h"
#include "Math.h"
#include <assert.h>

void OrdenarVerticesEnBloques(u32* Orden, u32* orden_bloques, u32 r, Grafo G)
{
    u32 n = NumeroDeVertices(G);

    // indices[c-1] tendrá el próximo indice en el cuál
    // se debe colocar un vértice del color c.
    u32* indices = calloc(r, sizeof(u32));

    // vert_por_color[c-1] contiene la cantidad de vertices
    // coloreados con el color c.
    u32* vert_por_color = calloc(r, sizeof(u32));
    for (u32 i = 0; i < n; ++i)
        vert_por_color[Color(i, G) - 1] += 1;

    // utilizamos el orden de los bloques de colores
    // llenar indices[] acorde a su definición.
    u32 acum = 0;
    for (u32 i = 0; i < r; ++i) {
        color c = orden_bloques[i];
        indices[c - 1] = acum;
        acum += vert_por_color[c - 1];
    }

    for (u32 i = 0; i < n; ++i) {
        // se guarda indice actual, luego se incrementa en 1.
        u32 ind = indices[Color(i, G) - 1]++;
        Orden[ind] = i;
    }

    free(indices);
    free(vert_por_color);
}

color CalcularMaxColor(Grafo G)
{
    u32 n = NumeroDeVertices(G);
    color max_color = 0;
    for (u32 v = 0; v < n; v++) {
        max_color = MAX(max_color, Color(v, G));
    }
    return max_color;
}

char ChequearOrden(u32* Orden, u32 n)
{
    char* found_flags = calloc(n, sizeof(char));
    if (found_flags == NULL) 
        return 0;

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
        } while (i < NumeroDeVertices(G) && Color(Orden[i], G) == c);
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

char ChequearGulDukat(Grafo G, u32* Orden)
{
    char rc = 1;
    u32 n = NumeroDeVertices(G);

    u32 r = 0;
    for (u32 i = 0; i < n; ++i)
        r = MAX(Color(i, G), r);

    u32* M = calloc(r, sizeof(color));
    u32* m = calloc(r, sizeof(color));

    for (u32 i = 0; i < r; ++i) {
        m[i] = n;
    }

    for (u32 i = 0; i < n; ++i) {
        u32 idx = Color(i, G) - 1;
        u32 grado = Grado(i, G);
        M[idx] = MAX(M[idx], grado);
        m[idx] = MIN(m[idx], grado);
    }

    char mul4 = (r >= 4) ? 1 : 0;
    char even = (4 > r && r >= 2) ? 1 : 0;
    char odd = (r == 1) ? 1 : 0;

    for (u32 i = 0; i < n - 1; ++i) {
        u32 v0 = Orden[i];
        u32 v1 = Orden[i + 1];
        color c0 = Color(v0, G);
        color c1 = Color(v1, G);

        if (c0 == c1)
            continue;

        if (mul4 && (c1 % 4 == 0)) {
            if (!(M[c0 - 1] >= M[c1 - 1])) {
                rc = 0;
                break;
            }
        } else if (mul4 && (c1 % 4 != 0) && (c1 % 2 == 0)) {
            mul4 = 0;
            even = 1;
        } else if (even && (c1 % 2 == 0)) {
            if (!(M[c0 - 1] + m[c0 - 1] >= M[c1 - 1] + m[c1 - 1])) {
                rc = 0;
                break;
            }
        } else if (even && (c1 % 2 != 0)) {
            even = 0;
            odd = 1;
        } else if (odd && (c1 % 2 == 1)) {
            if (!(m[c0 - 1] >= m[c1 - 1])) {
                rc = 0;
                break;
            }
        } else {
            rc = 0;
            break;
        }
    }

    free(m);
    free(M);
    return rc;
}
