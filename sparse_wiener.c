
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "graph.h"
#include "wiener.h"

void print_help();


int main( int argc , char **argv ){
    
    char topology[32];
    vector data = empty_vector;
    graph g = empty_graph;

    if( argc <= 1 ){
        print_help();
    }
    if( argc == 2 ){
        printf("Missing [DATA] field(s).\n\n");
        print_help();
    }

    strcpy( topology , argv[1] );
    for( int i = 2 ; i < argc ; i++ )
        vector_push_back_int( &data , atoi(argv[i]) );



    if( strcmp(topology,"-circular")==0 ){
        graph_init(&g , TOPOLOGY_CIRCULAR , &data );
    } else if( strcmp(topology,"-theta")==0 ){
        graph_init(&g , TOPOLOGY_THETA_GRAPH , &data );
    } else if( strcmp(topology,"-complete")==0 ){
        graph_init(&g , TOPOLOGY_COMPLETE_GRAPH , &data );
    } else if( strcmp(topology,"-star")==0 ){
        graph_init(&g , TOPOLOGY_STAR , &data );
    } else {
        printf("Unknown topology \"%s\".\n\n",topology);
        print_help();
        return 1;
    }

    printf( "%d\n" , W(&g) );

    return 0;
}

void print_help(){
    printf("Usage:\n\n");
    printf("sparse_wiener [TOPOLOGY] [DATA]\n\n\n");
    printf("[TOPOLOGY]:\n\n");
    printf("-circular           Circular topology. [DATA] then should be simply the number of vertices. \n");
    printf("-theta              Theta-graph topology. [DATA] then should be a sequence of m numbers, each representing the length of a bridge \n");
    printf("-complete           A complete graph. [DATA] then should be simply the number of vertices.\n");
    printf("-star               Star topology. [DATA] then should be a sequence of numbers, each representing the length of a bridge \n");
    exit(0);
}