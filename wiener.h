#ifndef WIENER_H
#define WIENER_H

#include <math.h>
#include <stdlib.h>

#include "list.h"
#include "vector.h"
#include "graph.h"

int W_floyd_warshall( graph *g , unsigned short ***dists );
int W( graph *g );
int W_b( bridge *b1 , int d );
int W_bc( bridge *b1 , int d1 , int d2 );
int W_bb( bridge *b1 , bridge *b2 , int da , int db , int dc , int dd );

#endif