
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "graph.h"
#include "wiener.h"

int main( int argc , char **argv ){
    int n = 6;
    if( argc >= 2 ) n = atoi(argv[1]);

    
    for( int i = 0 ; i < 10000 ; i++ ){

    graph g = empty_graph;
    vector data = empty_vector;
    vector_push_back_int(&data,444);
    vector_push_back_int(&data,555);
    vector_push_back_int(&data,333);
    graph_init( &g , TOPOLOGY_THETA_GRAPH , &data );


    printf("e vamo la:\n");
    printf("W_floyd_warshall(g)=%d\n" , W_floyd_warshall( &g , NULL ) );
    printf("W(g)=%d\n" , W( &g ) );

    graph_free( &g );
    vector_free( &data );

    }

    return 0;
}