// definiciones de GrafoSt, u32 y color.
#ifndef ESTRUCTURA_GRAFO
#define ESTRUCTURA_GRAFO

#include <stdint.h>

typedef uint32_t u32;
typedef u32 color;

typedef struct {
    u32* vecinos; // es NULL si y sólo si grado == 0
    u32 grado;
    u32 capacidad;
    color color;
} VerticeSt;

typedef struct {
    VerticeSt* vertices;
    u32 num_vertices;
    u32 num_lados;
    u32 delta;
} GrafoSt;

#endif