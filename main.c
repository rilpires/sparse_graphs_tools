
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "graph.h"
#include "wiener.h"

int main( int argc , char **argv ){
    int n = 6;
    if( argc >= 2 ) n = atoi(argv[1]);


    for( int i = 1 ; i < 200 ; i++ ){
        printf("n=%d\n",i);
        graph g = empty_graph;
        vector data = empty_vector;
        vector_push_back_int(&data,i);
        graph_init( &g , TOPOLOGY_RANDOM , &data );

        //graph_print( &g );

        int ret1 , ret2;

        printf("Starting W_floyd_warshall...\n");
        ret1 = W_floyd_warshall( &g  );
        printf("W_floyd_warshall(g)=%d\n" , ret1 );
        printf("Starting W(g)...\n");
        ret2 = W( &g );
        printf("W(g)=%d\n" , ret2 );

        if( ret1 != ret2 ){
            printf("shit");
        } else printf("Correct!\n\n");

        graph_clean( &g );
        vector_clean( &data );

    }

    return 0;
}