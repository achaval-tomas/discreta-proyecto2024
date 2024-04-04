#include "API2024Parte2.h"
#include <stdlib.h>

// se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden){
    u32 n = NumeroDeVertices(G);
    if (!CheckOrder(Orden, n))
        return -1;
}

char CheckOrder (u32* Orden, u32 length){
    char* flags = calloc(length, sizeof(char));

    for (u32 i = 0; i < length; ++i) {
        if (Orden[i] >= length)
            return 0;
        
        if (flags[Orden[i]])
            return 0;
        
        flags[Orden[i]] = 1;
    }

    free(flags);
    return 1;
}
