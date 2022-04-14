#include <stdio.h>
#include <sys/time.h>
#include <time.h>


#include "graph.h"
#include "partitions.h"
#include "wiener.h"

void print_help(){
    printf("Usage:\n");
    printf("thetagraph <total vertices> <number of bridges> [verbose]\n");
    exit(0);
}

int main( int argc , char** argv ){
    if( argc <= 2 ) print_help();
    struct timeval t0,t1;
    int verbose = 0;
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    if( argc >= 4 ){
        if( strcmp(argv[3],"verbose")==0 ){
            verbose=1;
        }
    }



    gettimeofday(&t0, 0);
    
    int min_W = 0x7FFFFFFF;
    vector min_datas = empty_vector;


    vector all_parts = empty_vector;
    generate_partitions(N-2,M,&all_parts);
    for( int i = 0 ; i < all_parts.size ; i++ ){
        vector* parts = vector_get_ptr(&all_parts,i);
        graph g = empty_graph;
        graph_init(&g,TOPOLOGY_THETA_GRAPH,parts);
        int Wg = W(&g , WIENER_METHOD_SPARSE_METHOD );
        if( min_W > Wg ){
            min_W = Wg;
            for( int j = 0 ; j < min_datas.size ; j++ ){
                vector* min_data = vector_get_ptr(&min_datas,j);
                vector_clean(min_data);
                free(min_data);
            }
            vector_clean(&min_datas);
        }
        if( min_W == Wg ){
            vector* min_data = malloc(sizeof(vector));
            *min_data = empty_vector;
            for( int j = 0 ; j < M ; j++ )
                vector_push_back_int(min_data,vector_get_int(parts,j));
            vector_push_back_ptr(&min_datas,min_data);
        }
        graph_clean(&g);
    }

    gettimeofday(&t1, 0);

    for( int i = 0 ; i < min_datas.size ; i++ ){
        vector* min_data = vector_get_ptr(&min_datas,i);
        printf("min W(g) =\t%d \t [ " , min_W );
        for( int j = 0 ; j < M ; j++ ){
            printf("%d " , vector_get_int(min_data,j));
        }
        printf("]\n");
    }
    if( verbose ){
        float elapsed = (t1.tv_sec - t0.tv_sec)*1000.0f + (t1.tv_usec-t0.tv_usec)/1000.0f;
        printf("Elapsed time: %.3f ms.\n",elapsed);
    }

}