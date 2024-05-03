#ifndef UTIL_H
#define UTIL_H

#include "API2024Parte2.h"

/*
Ordena los n vertices del grafo G por el orden de los colores en orden_bloques.
Se asume que Orden tiene n lugares de memoria y orden_bloques r.
Devuelve 0 en caso de éxito, 1 en caso contrario.
Complejidad: O(n)
*/
char OrdenarVerticesEnBloques(u32* Orden, u32* orden_bloques, u32 r, Grafo G);

/*
G debe ser un grafo previamente coloreado por Greedy.
Complejidad: O(n)
*/
color CalcularMaxColor(Grafo G);

/*
Se verifica que Orden sea un ordenamiento de vértices válido,
es decir que sea una permutación del conjunto {0, 1, ..., n-1}
Complejidad: O(n)
*/
char ChequearOrden(u32* Orden, u32 n);

/* Funciones de testeo */
char ChequearReordenamientoVIT(Grafo G, u32* Orden);
char ChequearGulDukat(Grafo G, u32* Orden);

#endif
