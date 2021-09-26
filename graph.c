#include "graph.h"
#include "stdio.h"

void _graph_init( graph* g , int n ){
    vector_resize( &g->adjs , n );
    for( int i = 0 ; i < n ; i++ ){
        g->adjs.values[i].ptr_value = malloc(sizeof(vector));
        *g->adjs.values[i].vec_value = empty_vector;
    }
}
void _graph_connect_vertexes( graph* g , int v1 , int v2 ){
    vector_push_back_int( g->adjs.values[v1].vec_value , v2 );
    vector_push_back_int( g->adjs.values[v2].vec_value , v1 );
}

void graph_init( graph* g , int topology , vector* data){
    switch (topology)
    {

    case TOPOLOGY_CIRCULAR:{
        int n = vector_get_int(data,0);
        _graph_init(g,n);
        for( int i = 0 ; i < n ; i++ ){
            _graph_connect_vertexes( g , i , (i+1)%n );
        }
    }break;
    


    case TOPOLOGY_THETA_GRAPH:{
        int n = 2;
        int m = data->size;
        for( int i = 0 ; i < m ; i++ ){
            n += vector_get_int(data,i);
        }
        _graph_init(g,n);
        int open_index = 1;
        for( int bi = 0 ; bi < m ; bi ++ ){
            int bridge_size = vector_get_int(data,bi);
            if( bridge_size > 0 ){
                _graph_connect_vertexes( g , 0 , open_index );
                for( int i = 1 ; i < bridge_size ; i++ ){
                    _graph_connect_vertexes( g , open_index , open_index+1 );
                    open_index++;
                }
                _graph_connect_vertexes( g , open_index , n-1 );
                open_index++;
            } else {
                _graph_connect_vertexes( g , 0 , n-1 );
            }
        }

    }break;

    default: printf("Use one of the values for TOPOLOGY_*\n") ; break;
    
    }
}

void graph_free( graph* g ){
    int n = g->adjs.size;
    int bn = g->bridges.size;
    for( int i = 0 ; i < n ; i++ ){
        vector_free( g->adjs.values[i].vec_value );
        free(g->adjs.values[i].vec_value);
    }
    for( int i = 0 ; i < g->bridge_map.size ; i++ ){
        vector_free( g->bridge_map.values[i].vec_value );
        free(g->bridge_map.values[i].vec_value);
    }
    for( int i = 0 ; i < bn ; i++ ){
        bridge* br = g->bridges.values[i].ptr_value;
        vector_free( &br->vertexes );
        free(br);
    }

    vector_free( &g->adjs );
    vector_free( &g->bridge_map );
    vector_free( &g->bridges );
}
