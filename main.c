
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
    vector_push_back_int(&data,113);
    vector_push_back_int(&data,115);
    vector_push_back_int(&data,1111);
    vector_push_back_int(&data,111);
    vector_push_back_int(&data,115);
    graph_init( &g , TOPOLOGY_THETA_GRAPH , &data );


    printf("Starting W_floyd_warshall...\n");
    printf("W_floyd_warshall(g)=%d\n" , W_floyd_warshall( &g , NULL ) );
    printf("Starting W(g)...");
    printf("W(g)=%d\n" , W( &g ) );

    graph_free( &g );
    vector_free( &data );


    return 0;
}