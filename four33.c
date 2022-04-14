#include <stdio.h>


#include "graph.h"
#include "partitions.h"
#include "wiener.h"

void print_help(){
    printf("Usage:\n");
    printf("four33 <total vertices> [verbose]\n");
    exit(0);
}

int main( int argc , char** argv ){
    if( argc <= 1 ) print_help();
    int verbose = 0;
    int N = atoi(argv[1]);
    int N_inner = N-3;
    if( argc >= 3 ){
        if( strcmp(argv[2],"verbose")==0 ){
            verbose=1;
        }
    }

    int min_W = 0x7FFFFFFF;
    vector min_datas = empty_vector;


    for( int X = 0 ; X <= N_inner ; X++ ){
        int Y = N_inner - X;
        for( int X1 = 0 ; X1 <= X/2 ; X1++ ){
            int X2 = X - X1;
            for( int X11 = 0 ; X11 <= X1/2 ; X11++ ){
                for( int X21 = 0 ; X21 <= X2/2 ; X21++ ){
                    int X12 = X1 - X11;
                    int X22 = X2 - X21;
                    
                    vector data = empty_vector;
                    graph g = empty_graph;
                    vector_push_back_int(&data,N);
                    graph_init(&g,TOPOLOGY_EMPTY,&data);


                    int open_index = 3;
                    
                    if( X11 == 0 ) graph_connect_vertices( &g , 0 , 1 );
                    else {
                        graph_connect_vertices( &g , 0 , open_index );
                        for( int i = 1 ; i < X11 ; i++ , open_index++ ){
                            graph_connect_vertices( &g , open_index , open_index+1 );
                        }
                        graph_connect_vertices(&g , open_index++ , 1 );
                    }
                    
                    if( X12 == 0 ) graph_connect_vertices( &g , 0 , 1 );
                    else {
                        graph_connect_vertices( &g , 0 , open_index );
                        for( int i = 1 ; i < X12 ; i++ , open_index++ ){
                            graph_connect_vertices( &g , open_index , open_index+1 );
                        }
                        graph_connect_vertices(&g , open_index++ , 1 );
                    }


                    if( X21 == 0 ) graph_connect_vertices( &g , 0 , 2 );
                    else {
                        graph_connect_vertices( &g , 0 , open_index );
                        for( int i = 1 ; i < X21 ; i++ , open_index++ ){
                            graph_connect_vertices( &g , open_index , open_index+1 );
                        }
                        graph_connect_vertices(&g , open_index++ , 2 );
                    }

                    if( X22 == 0 ) graph_connect_vertices( &g , 0 , 2 );
                    else {
                        graph_connect_vertices( &g , 0 , open_index );
                        for( int i = 1 ; i < X22 ; i++ , open_index++ ){
                            graph_connect_vertices( &g , open_index , open_index+1 );
                        }
                        graph_connect_vertices(&g , open_index++ , 2 );
                    }

                    if( Y == 0 ) graph_connect_vertices( &g , 1 , 2 );
                    else {
                        graph_connect_vertices( &g , 1 , open_index );
                        for( int i = 1 ; i < Y ; i++ , open_index++ ){
                            graph_connect_vertices( &g , open_index , open_index+1 );
                        }
                        graph_connect_vertices(&g , open_index++ , 2 );
                    }

                    int Wg = W(&g , WIENER_METHOD_SPARSE_METHOD);

                    
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
                        vector_push_back_int( min_data , Y );
                        vector_push_back_int( min_data , X11 );
                        vector_push_back_int( min_data , X12 );
                        vector_push_back_int( min_data , X21 );
                        vector_push_back_int( min_data , X22 );
                        vector_push_back_ptr(&min_datas,min_data);
                    }

                    graph_clean(&g);
                    vector_clean(&data);

                }
            }
        }
    }


    for( int i = 0 ; i < min_datas.size ; i++ ){
        vector* min_data = vector_get_ptr(&min_datas,i);
        printf("min W(g) =\t%d \t [ " , min_W );
        for( int j = 0 ; j < min_data->size ; j++ ){
            printf("%d " , vector_get_int(min_data,j));
        }
        printf("]\n");
    }

}