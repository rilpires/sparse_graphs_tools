
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "graph.h"
#include "wiener.h"

int main( int argc , char **argv ){
    int n = 6;
    if( argc >= 2 ) n = atoi(argv[1]);

    

    graph g = empty_graph;
    vector data = empty_vector;
    vector_push_back_int(&data,44);
    vector_push_back_int(&data,55);
    vector_push_back_int(&data,33);
    graph_init( &g , TOPOLOGY_THETA_GRAPH , &data );


    printf("W_floyd_warshall(g)=%d\n" , W_floyd_warshall( &g , NULL ) );
    printf("W(g)=%d\n" , W( &g ) );



    return 0;
}