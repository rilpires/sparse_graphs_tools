#include <stdio.h>


#include "graph.h"
#include "partitions.h"
#include "wiener.h"

void print_help(){
    printf("Usage:\n");
    printf("tetrahedron N [verbose]\n");
    exit(0);
}
int main( int argc , char** argv ){
    if( argc <= 1 ) print_help();
    int N = atoi(argv[1]);
    int verbose = 0;
    if( argc > 2 ){
        if( strcmp(argv[2],"verbose")==0 ){
            verbose=1;
        }
    }

    int min_W = 0x7FFFFFFF;
    vector min_datas = empty_vector;

    for( int X = 0 ; X <= (N-4)/2 ; X++ ){
        int Y = (N-4)-X;
        vector vx = empty_vector;
        vector vy = empty_vector;

        generate_partitions(X,3,&vx);
        generate_partitions(Y,3,&vy);
        for( int i = 0 ; i < vx.size ; i++ ){
            for( int j = 0 ; j < vy.size ; j++ ){
                vector* vx_parts = vector_get_ptr(&vx,i);
                vector* vy_parts = vector_get_ptr(&vy,j);

                int A = vector_get_int(vy_parts,0);
                int B = vector_get_int(vy_parts,1);
                int C = vector_get_int(vy_parts,2);
                int D = vector_get_int(vx_parts,0);
                int E = vector_get_int(vx_parts,1);
                int F = vector_get_int(vx_parts,2);

                graph g1 = empty_graph;
                graph g2 = empty_graph;
                graph g3 = empty_graph;
                graph g4 = empty_graph;
                graph g5 = empty_graph;
                graph g6 = empty_graph;

                vector g1_data = empty_vector;
                vector g2_data = empty_vector;
                vector g3_data = empty_vector;
                vector g4_data = empty_vector;
                vector g5_data = empty_vector;
                vector g6_data = empty_vector;

                // The first 3 are always the same
                vector_push_back_int( &g1_data , A );
                vector_push_back_int( &g1_data , B );
                vector_push_back_int( &g1_data , C );
                vector_push_back_int( &g2_data , A );
                vector_push_back_int( &g2_data , B );
                vector_push_back_int( &g2_data , C );
                vector_push_back_int( &g3_data , A );
                vector_push_back_int( &g3_data , B );
                vector_push_back_int( &g3_data , C );
                vector_push_back_int( &g4_data , A );
                vector_push_back_int( &g4_data , B );
                vector_push_back_int( &g4_data , C );
                vector_push_back_int( &g5_data , A );
                vector_push_back_int( &g5_data , B );
                vector_push_back_int( &g5_data , C );
                vector_push_back_int( &g6_data , A );
                vector_push_back_int( &g6_data , B );
                vector_push_back_int( &g6_data , C );

                // Now the order changes
                vector_push_back_int( &g1_data , D );
                vector_push_back_int( &g1_data , E );
                vector_push_back_int( &g1_data , F );
                vector_push_back_int( &g2_data , D );
                vector_push_back_int( &g2_data , F );
                vector_push_back_int( &g2_data , E );
                vector_push_back_int( &g3_data , E );
                vector_push_back_int( &g3_data , F );
                vector_push_back_int( &g3_data , D );
                vector_push_back_int( &g4_data , E );
                vector_push_back_int( &g4_data , D );
                vector_push_back_int( &g4_data , F );
                vector_push_back_int( &g5_data , F );
                vector_push_back_int( &g5_data , E );
                vector_push_back_int( &g5_data , D );
                vector_push_back_int( &g6_data , F );
                vector_push_back_int( &g6_data , D );
                vector_push_back_int( &g6_data , E );


                graph_init( &g1 , TOPOLOGY_TETRAHEDRON , &g1_data );
                graph_init( &g2 , TOPOLOGY_TETRAHEDRON , &g2_data );
                graph_init( &g3 , TOPOLOGY_TETRAHEDRON , &g3_data );
                graph_init( &g4 , TOPOLOGY_TETRAHEDRON , &g4_data );
                graph_init( &g5 , TOPOLOGY_TETRAHEDRON , &g5_data );
                graph_init( &g6 , TOPOLOGY_TETRAHEDRON , &g6_data );

                int W_g1 = W(&g1 , WIENER_METHOD_SPARSE_METHOD );
                int W_g2 = W(&g2 , WIENER_METHOD_SPARSE_METHOD );
                int W_g3 = W(&g3 , WIENER_METHOD_SPARSE_METHOD );
                int W_g4 = W(&g4 , WIENER_METHOD_SPARSE_METHOD );
                int W_g5 = W(&g5 , WIENER_METHOD_SPARSE_METHOD );
                int W_g6 = W(&g6 , WIENER_METHOD_SPARSE_METHOD );

                #define PRINT_G(g) printf("W(g) = %d \t [%d %d %d %d %d %d]\n" , W_##g , vector_get_int(&g##_data,0) , vector_get_int(&g##_data,1) , vector_get_int(&g##_data,2) , vector_get_int(&g##_data,3) , vector_get_int(&g##_data,4) , vector_get_int(&g##_data,5) );
                
                #define CHECK_FOR_MIN(g)                                                \
                if( W_##g < min_W ){                                                    \
                    min_W = W_##g;                                                      \
                    for( int k = 0 ; k < min_datas.size ; k++ ){                        \
                        vector* min_data = vector_get_ptr(&min_datas,k);                \
                        vector_clean(min_data);                                         \
                        free(min_data);                                                 \
                    }                                                                   \
                    vector_clean(&min_datas);                                           \
                }                                                                       \
                if( min_W == W_##g ){                                                   \
                    vector* min_data = malloc(sizeof(vector));                          \
                    *min_data = empty_vector;                                           \
                    for( int k = 0 ; k < 6 ; k++ )                                      \
                        vector_push_back_int( min_data , vector_get_int(&g##_data,k) ); \
                    vector_push_back_ptr(&min_datas,min_data);                          \
                }

                if(verbose){
                    PRINT_G(g1);
                    PRINT_G(g2);
                    PRINT_G(g3);
                    PRINT_G(g4);
                    PRINT_G(g5);
                    PRINT_G(g6);
                }

                CHECK_FOR_MIN(g1);
                CHECK_FOR_MIN(g2);
                CHECK_FOR_MIN(g3);
                CHECK_FOR_MIN(g4);
                CHECK_FOR_MIN(g5);
                CHECK_FOR_MIN(g6);

                graph_clean(&g1);
                graph_clean(&g2);
                graph_clean(&g3);
                graph_clean(&g4);
                graph_clean(&g5);
                graph_clean(&g6);

            }
        }

        vector_clean(&vx);
        vector_clean(&vy);

    }

    // Now, removing duplicated entries in min_datas... actually, turning the duplicated entries into NULL
    for( int i = 0 ; i < min_datas.size ; i++ ){
        for( int j = i+1 ; j < min_datas.size ; j++ ){
            vector* v1 = vector_get_ptr(&min_datas,i);
            vector* v2 = vector_get_ptr(&min_datas,j);
            if( vector_compare(v1,v2)==0 ){
                vector_set_ptr(&min_datas,j,NULL);
                vector_clean(v2);
                free(v2);
            }
        }
    }

    for( int i = 0 ; i < min_datas.size ; i++ ){
        vector* min_data = vector_get_ptr(&min_datas,i);
        if( min_data ) printf("min W(g) =\t%d \t [%d %d %d %d %d %d]\n" , min_W , vector_get_int(min_data,0) , vector_get_int(min_data,1) , vector_get_int(min_data,2) , vector_get_int(min_data,3) , vector_get_int(min_data,4) , vector_get_int(min_data,5) );
    }

}