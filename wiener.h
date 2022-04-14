#ifndef WIENER_H
#define WIENER_H

#include <math.h>
#include <stdlib.h>

#include "list.h"
#include "vector.h"
#include "graph.h"


enum WIENER_METHOD {
    WIENER_METHOD_FLOYD_WARSHALL , 
    WIENER_METHOD_MULTIPLE_DIJKSTRAS , 
    WIENER_METHOD_SPARSE_METHOD , 
};

int W( graph *g , enum WIENER_METHOD which_method );

int _W_b( bridge *b1 , unsigned int d );
int _W_bc( bridge *b1 , unsigned int d1 , unsigned int d2 );
int _W_bb( bridge *b1 , bridge *b2 , unsigned int da , unsigned int db , unsigned int dc , unsigned int dd );

#endif