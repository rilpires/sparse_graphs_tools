#include <stdio.h>


#include "graph.h"
#include "partitions.h"
#include "wiener.h"

void print_help(){
    printf("Usage:\n");
    printf("thetagraph <total vertices> <number of bridges> [verbose]\n");
    exit(0);
}
void print_data();
int main( int argc , char** argv ){
    if( argc <= 2 ) print_help();
    int verbose = 0;
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    if( argc >= 4 ){
        if( strcmp(argv[3],"verbose")==0 ){
            verbose=1;
        }
    }

    int min_W = 0x7FFFFFFF;
    vector min_data = empty_vector;


    vector all_parts = empty_vector;
    generate_partitions(N-2,M,&all_parts);
    for( int i = 0 ; i < all_parts.size ; i++ ){
        vector* parts = vector_get_ptr(&all_parts,i);
        graph g = empty_graph;
        graph_init(&g,TOPOLOGY_THETA_GRAPH,parts);
        int Wg = W(&g);
        if( min_W >= Wg ){
            min_W = Wg;
            vector_clean(&min_data);
            for( int j = 0 ; j < M ; j++ )
                vector_push_back_int(&min_data,vector_get_int(parts,j));
        }
        graph_clean(&g);
    }
    
    printf("min W(g) = %d \t [ " , min_W );
    for( int i = 0 ; i < M ; i++ ){
        printf("%d " , vector_get_int(&min_data,i));
    }
    printf("]\n");

}