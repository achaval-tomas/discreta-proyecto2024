#ifndef API2024_PARTE_2_H
#define API2024_PARTE_2_H

#include "APIG24.h"

// se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden);

// funciones para crear ordenes.
char GulDukat(Grafo G, u32* Orden);
char ElimGarak(Grafo G, u32* Orden);

#endif /* API2024_PARTE_2 */