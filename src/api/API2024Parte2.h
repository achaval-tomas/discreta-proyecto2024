#ifndef API2024_PARTE_2_H
#define API2024_PARTE_2_H

#include "APIG24.h"

// Se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden);

/*
Se asume que Orden apunta a una region de memoria con n lugares y
que G tiene un coloreo propio con colores {1, 2, .., r} para algún r.
Complejidad: O(n)
 */
char GulDukat(Grafo G, u32* Orden);

/*
Se asume que Orden apunta a una region de memoria con n lugares y
que G tiene un coloreo propio con colores {1, 2, .., r} para algún r.
Complejidad: O(n)
 */
char ElimGarak(Grafo G, u32* Orden);

#endif /* API2024_PARTE_2_H */