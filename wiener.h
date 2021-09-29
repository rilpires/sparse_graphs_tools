#ifndef WIENER_H
#define WIENER_H

#include <math.h>
#include <stdlib.h>

#include "list.h"
#include "vector.h"
#include "graph.h"

int W_floyd_warshall( graph *g  );
int W( graph *g );
int W_b( bridge *b1 , unsigned int d );
int W_bc( bridge *b1 , unsigned int d1 , unsigned int d2 );
int W_bb( bridge *b1 , bridge *b2 , unsigned int da , unsigned int db , unsigned int dc , unsigned int dd );

#endif