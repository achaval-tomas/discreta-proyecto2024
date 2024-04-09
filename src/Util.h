#ifndef UTIL_H
#define UTIL_H

#include "API2024Parte2.h"

/*
G debe ser un grafo previamente coloreado por Greedy.
Complejidad: O(n)
*/
u32 CalcularMaxColor(Grafo G);

char ChequearOrden(u32* Orden, u32 n);
char ChequearReordenamientoVIT(Grafo G, u32* Orden);

#endif
