#include "graph.h"
#include "list.h"
#include "stdio.h"

void _graph_init( graph* g , int n ){
    vector_resize( &g->adjs , n );
    for( int i = 0 ; i < n ; i++ ){
        g->adjs.values[i].ptr_value = malloc(sizeof(vector));
        *g->adjs.values[i].vec_value = empty_vector;
    }
    vector_resize( &g->bridge_map , n );
    vector_clean( &g->aux_to_original );
    vector_resize( &g->original_to_aux , n );
}

void graph_connect_vertices( graph* g , int v1 , int v2 ){
    vector_push_back_int( g->adjs.values[v1].vec_value , v2 );
    vector_push_back_int( g->adjs.values[v2].vec_value , v1 );
}

void graph_connect_vertices_safe( graph* g , int v1 , int v2 ){
    if( !g
        || v1==v2 
        || g->adjs.size <= MAX(v1,v2)
        || vector_contains( g->adjs.values[v1].ptr_value , v2 ) 
    ) return;
    graph_connect_vertices( g , v1 , v2 );
}

void graph_init( graph* g , int topology , vector* data){
    switch (topology)
    {

    case TOPOLOGY_CIRCULAR:{
        int n = vector_get_int(data,0);
        _graph_init(g,n);
        for( int i = 0 ; i < n ; i++ ){
            graph_connect_vertices( g , i , (i+1)%n );
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
                graph_connect_vertices( g , 0 , open_index );
                for( int i = 1 ; i < bridge_size ; i++ ){
                    graph_connect_vertices( g , open_index , open_index+1 );
                    open_index++;
                }
                graph_connect_vertices( g , open_index , n-1 );
                open_index++;
            } else {
                graph_connect_vertices( g , 0 , n-1 );
            }
        }

    }break;

    case TOPOLOGY_SINGLE_BRIDGE:{
        int n = vector_get_int(data,0);
        _graph_init(g,n);
        for( int i = 0 ; i < n-1 ; i++ ){
            graph_connect_vertices( g , i , i+1 );
        }
    }break;

    case TOPOLOGY_COMPLETE_GRAPH:{
        int n = vector_get_int(data,0);
        _graph_init(g,n);
        for( int i = 0   ; i < n ; i++ )
        for( int j = i+1 ; j < n ; j++ )
            graph_connect_vertices( g , i , j );
    }break;

    case TOPOLOGY_STAR:{
        int n = 1;
        int m = data->size;
        int open_index = 1;
        for( int i = 0 ; i < m ; i++ )
            n += vector_get_int(data,i);
        _graph_init(g,n);
        for( int i = 0 ; i < m ; i++ ){
            graph_connect_vertices(g,0,open_index);
            open_index++;
            for( int j=0 ; j<vector_get_int(data,i)-1 ; j++ ){
                graph_connect_vertices(g,open_index-1,open_index);
                open_index++;
            }
        }
        while( !graph_is_connected(g) )
            graph_connect_vertices_safe( g , rand()%n , rand()%n );
    }break;

    case TOPOLOGY_RANDOM:{
        int n = vector_get_int(data,0);
        _graph_init(g,n);
        while( !graph_is_connected(g) )
            graph_connect_vertices_safe( g , rand()%n , rand()%n );
    }break;

    default: printf("Use one of the values for TOPOLOGY_*\n") ; break;
    
    }
}


int graph_is_connected( graph* g ){
    int n = g->adjs.size;
    vector visiteds = empty_vector;
    vector_resize(&visiteds,n);
    for( int i = 0 ; i < n ; i++ )
        visiteds.values[i].int_value = 0;
    list to_visit = empty_list;
    list_push_back( &to_visit , 0 );
    while( to_visit.first ){
        list_node* v = to_visit.first;
        
        visiteds.values[v->val].int_value = 1;

        int degree = g->adjs.values[v->val].vec_value->size;
        for( int i = 0 ; i < degree ; i++ ){
            int neighb = g->adjs.values[v->val].vec_value->values[i].int_value;
            if( visiteds.values[neighb].int_value == 0 ){
                list_push_back( &to_visit , neighb );
            }
        }
        list_pop_front( &to_visit );

    }
    list_clean(&to_visit);

    for( int i = 0 ; i < n ; i++ )
    if( visiteds.values[i].int_value == 0 ){
        vector_clean(&visiteds);
        return 0;
    }
    vector_clean(&visiteds);
    return 1;

}


void graph_clean( graph* g ){
    int n = g->adjs.size;
    int aux_n = g->aux_to_original.size;
    int bn = g->bridges.size;
    for( int i = 0 ; i < n ; i++ ){
        vector_clean( g->adjs.values[i].vec_value );
        free(g->adjs.values[i].vec_value);
    }
    for( int i = 0 ; i < bn ; i++ ){
        bridge* br = g->bridges.values[i].ptr_value;
        vector_clean( &br->vertices );
        free(br);
    }
    if(g->aux_dists){
        for( int i = 0 ; i < aux_n ; i++ )
            free(g->aux_dists[i]);
        free(g->aux_dists);
    }
    if( g->dists ){ 
        for( int i = 0 ; i < n ; i++ )
            free(g->dists[i]);
        free(g->dists);
    }
    vector_clean( &g->adjs );
    vector_clean( &g->bridge_map );
    vector_clean( &g->bridges );
    vector_clean( &g->aux_to_original );
    vector_clean( &g->original_to_aux );
    
    *g = empty_graph;
}

void graph_print( graph* g ){
    int n = g->adjs.size;
    
    printf("\n");
    for( int i = 0 ; i < n ; i++ ){
        printf("[%d]: " , i );
        for( int ii = 0 ; ii < g->adjs.values[i].vec_value->size ; ii++ ){
            int j = g->adjs.values[i].vec_value->values[ii].int_value;
            if( ii == 0 )   printf("%d" , j );
            else            printf(", %d" , j );
        }
        printf("\n");
    }
}
