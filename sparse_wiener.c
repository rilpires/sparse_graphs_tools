
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>


#include "graph.h"
#include "wiener.h"

void print_help();


int main( int argc , char **argv ){
    
    char topology[32];
    vector data = empty_vector;
    graph g = empty_graph;
    int use_floyd_warshall=0;
    int show_time=0;
    struct timeval t0,t1;

    argc--; argv++;
    while( argc>2 ){
        if( argv[0][0]!='-' ) break;
        else if( strcmp(argv[0],"--verbose")==0 || strcmp(argv[0],"-v")==0 ){
            show_time = 1;
        }
        else if( strcmp(argv[0],"--floyd")==0 || strcmp(argv[0],"-f")==0 ){
            use_floyd_warshall = 1;
        }
        else{
            printf("Unrecognized option: %s\n\n",argv[0]);
            print_help();
            exit(1);
        }
        argc--; argv++;
    }
    if( argc < 2 ) print_help();

    strcpy( topology , argv[0] );
    for( int i = 1 ; i < argc ; i++ )
        vector_push_back_int( &data , atoi(argv[i]) );
    
    if( strcmp(topology,"circular")==0 ){
        graph_init(&g , TOPOLOGY_CIRCULAR , &data );
    } else if( strcmp(topology,"theta")==0 ){
        graph_init(&g , TOPOLOGY_THETA_GRAPH , &data );
    } else if( strcmp(topology,"complete")==0 ){
        graph_init(&g , TOPOLOGY_COMPLETE_GRAPH , &data );
    } else if( strcmp(topology,"tetrahedron")==0 ){
        graph_init(&g , TOPOLOGY_TETRAHEDRON , &data );
    } else if( strcmp(topology,"star")==0 ){
        graph_init(&g , TOPOLOGY_STAR , &data );
    } else if( strcmp(topology,"random")==0 ){
        graph_init(&g , TOPOLOGY_RANDOM , &data );
    } else {
        printf("Unknown topology \"%s\".\n\n",topology);
        print_help();
        return 1;
    }


    int ret;
    gettimeofday(&t0, 0);
    if( use_floyd_warshall ) ret = W_floyd_warshall(&g);
    else ret = W(&g);
    gettimeofday(&t1, 0);
    
    printf( "%d\n" , ret );
    if(show_time){
        float elapsed = (t1.tv_sec - t0.tv_sec)*1000.0f + (t1.tv_usec-t0.tv_usec)/1000.0f;
        printf("elapsed time: %f ms\n" , elapsed );
    }
    
    return 0;
}

void print_help(){
    printf("Usage:\n\n");
    printf("sparse_wiener [OPTIONS] <TOPOLOGY> <DATA>\n\n");
    printf("\n[OPTIONS]:\n");
    printf("--verbose , -v      Show time log\n");
    printf("--floyd , -f        Use Floyd-Warshall algorithm(not default)\n");
    printf("\n<TOPOLOGY>:\n");
    printf("circular            Circular topology. <DATA> then should be simply the number of vertices. \n");
    printf("theta               Theta-graph topology. <DATA> then should be a sequence of m numbers, each representing the length of a bridge \n");
    printf("complete            A complete graph. <DATA> then should be simply the number of vertices.\n");
    printf("star                Star topology. <DATA> then should be a sequence of numbers, each representing the length of a bridge \n");
    printf("tetrahedron         Tetrahedron topology. <DATA> then should be a sequence of 6 numbers, each representing the amount of vertices in each bridge \n");
    printf("random              An arbitrarily generated graph. <DATA> then should be 2 numbers: the number of vertices and a seed\n");
    exit(0);
}