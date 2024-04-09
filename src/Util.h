#ifndef UTIL_H
#define UTIL_H

#include "API2024Parte2.h"

/*
Ordena los n vertices del grafo G por el orden de los colores en orden_bloques.
se asume que Orden tiene n lugares de memoria y orden_bloques r.
*/
void OrdenarVerticesEnBloques(u32* Orden, u32* orden_bloques, u32 r, Grafo G);

/*
G debe ser un grafo previamente coloreado por Greedy.
Complejidad: O(n)
*/
u32 CalcularMaxColor(Grafo G);

char ChequearOrden(u32* Orden, u32 n);
char ChequearReordenamientoVIT(Grafo G, u32* Orden);

#endif
