#include <math.h>
#include <stdio.h>
#include <string.h>

#include "wiener.h"

#define MIN(a,b) ((a<b)?(a):(b))


unsigned short** _adj_matrix( graph *g ){
    int n = g->adjs.size;
    unsigned short **dists;
    
    dists = malloc( sizeof(*dists) * n );

    for( int i = 0 ; i < n ; i++ ){
        dists[i] = malloc( sizeof(dists[0][0]) * n );
        memset(dists[i] , 0 , sizeof(dists[0][0]) * n );
    }

    for( int i = 0 ; i < n ; i++ ){
        for( int k = 0 ; k < g->adjs.values[i].vec_value->size ; k++ ){
            int j = g->adjs.values[i].vec_value->values[k].int_value;
            dists[i][j] = 1;
        }
    }
    return dists;
}
void _fill_dist_matrix( unsigned short **adjs , unsigned short **dists , int n ){
    for( int i = 0 ; i < n ; i++ ){
        dists[i][i] = 0;
        for( int j = i+1 ; j < n ; j++ ){
            switch (adjs[i][j]) {
                case 0:
                    dists[i][j] = -1;
                    break;
                default:
                    dists[i][j] = adjs[i][j];
                    break;
            }
        }        
    }

    for( int k = 0 ; k < n ; k++ ){
        for( int i = 0 ; i < n ; i++ ){
            for( int j = i+1 ; j < n ; j++ ){
                unsigned short dist_ki = (k<i) ? (dists[k][i]) : (dists[i][k]) ;
                unsigned short dist_kj = (k<j) ? (dists[k][j]) : (dists[j][k]) ;
                dists[i][j] = MIN( dists[i][j] , dist_ki + dist_kj ) ;
            }
        }   
    }
}
void _fill_step( graph* g , vector* visiteds , int current ){
    
    int degree = g->adjs.values[current].vec_value->size;
    vector_set_int(visiteds,current,1);
    
    if(degree==0) {
        printf("wtf?\n");
    } else if(degree<=2) {
        bridge* new_bridge = malloc(sizeof(bridge));
        new_bridge->vertexes = empty_vector;
        new_bridge->extremity_0 = -1;
        new_bridge->extremity_1 = -1;
        int bridge_index = g->bridges.size;
        vector_push_back_ptr( &g->bridges , new_bridge );
        vector_push_back_int( &new_bridge->vertexes , current );
        vector_push_back_int( g->bridge_map.values[current].vec_value , bridge_index );

        // fill this entire bridge now. Trust me, thats easier
        list to_visit = empty_list;
        // this list is to recursive call later, on nodes with degree>2 which isnt in any bridge (it will be 1 or 2)
        list to_visit_later = empty_list; 

        for( int i = 0 ; i < degree ; i++ ){
            list_push_back( &to_visit , g->adjs.values[current].vec_value->values[i].int_value );
        }
        while( to_visit.first ){
            int visiting = to_visit.last->val;
            list_pop_back(&to_visit);
            
            vector* visiting_neighboors = vector_get_ptr( &g->adjs , visiting );
            int visiting_degree = visiting_neighboors->size;
            
            if( visiting_degree <= 2 ){
                if( vector_get_int(visiteds,visiting) == 0) {
                    vector_set_int( visiteds,visiting,1);
                    vector_push_back_int( &new_bridge->vertexes , visiting );
                    vector_push_back_int( g->bridge_map.values[visiting].vec_value , bridge_index );
                    for( int i = 0 ; i<visiting_degree ; i++ ){
                        int visit2 = visiting_neighboors->values[i].int_value; 
                        list_push_back(&to_visit,visit2);
                    }
                }
            } else { 
                if(new_bridge->extremity_0==-1) new_bridge->extremity_0 = visiting;
                else if(new_bridge->extremity_1==-1) new_bridge->extremity_1 = visiting;
                if( vector_get_int(visiteds,visiting) == 0) {
                    list_push_back(&to_visit_later,visiting);
                }
            }    
        }

        // Now we go recursive on all this bridge discovering
        for( list_node* l = to_visit_later.first ; l ; l = l->next ){
            if( vector_get_int(visiteds,l->val) == 0 ){
                _fill_step( g , visiteds , l->val );
            }
        }

        list_free(&to_visit);
        list_free(&to_visit_later);
    } 
    // Just recursive into neighboors
    else {
        for( int i = 0 ; i < degree ; i++ ){
            int next_visit = vector_get_int(g->adjs.values[current].vec_value,i);
            if( vector_get_int(visiteds,next_visit) == 0 ){
                _fill_step( g , visiteds , next_visit );
            }
        }
    }
}
void _fill_bridges( graph *g ){

    int n = g->adjs.size;
    g->bridge_map = empty_vector;
    g->bridges = empty_vector;
    vector_resize( &g->bridge_map , n );
    for( int i = 0 ; i < n ; i++ ){
        g->bridge_map.values[i].ptr_value = malloc(sizeof(vector));
        *g->bridge_map.values[i].vec_value = empty_vector;
    }

    vector visiteds = empty_vector;
    vector_resize( &visiteds , n );
    _fill_step( g , &visiteds , 0 );
    vector_free( &visiteds );
}


int W_floyd_warshall( graph *g  , unsigned short ***p_dists){
    int n = g->adjs.size;
    unsigned short **dists;
    unsigned short **adj_matrix;
    int free_later = 0;
    if( p_dists == NULL ){
        dists = malloc( sizeof(*dists) * n );
        free_later = 1;
    } else {
        *p_dists = malloc( sizeof(p_dists[0]) * n );
        dists = *p_dists;
    }

    for( int i = 0 ; i < n ; i++ ){
        dists[i] = malloc( sizeof(dists[0][0]) * n );
        memset(dists[i] , -1 , sizeof(dists[0][0]) * n );
    }

    adj_matrix = _adj_matrix(g);

    _fill_dist_matrix(adj_matrix , dists , n );

    int ret = 0 ;
    for( int i = 0 ; i < n ; i++ ){
        for( int j = i+1 ; j < n ; j++ ){
            ret += dists[i][j];
        }
    }

    if(free_later) free(dists);
    free(adj_matrix);

    return 2*ret;
}

int W( graph *g ){
    int n = g->adjs.size;

    _fill_bridges(g);

    for( int bi = 0 ; bi < g->bridges.size ; bi++ ){
        bridge* b = g->bridges.values[bi].ptr_value;
        for( int i = 0 ; i < b->vertexes.size ; i++ ){
            printf("ponte %d tem o vertex %d\n" , bi , b->vertexes.values[i].int_value );
        }
    }

    vector all_degs2_below = empty_vector , all_degs3_above = empty_vector;
    vector degs3_original_table = empty_vector; // this is all_degs3_above reverted (index->key), so we can fast recover the key from the other
    vector_resize(&degs3_original_table , n);
    for( int i = 0 ; i < n ; i++ ){
        if( g->adjs.values[i].vec_value->size <= 2 ){
            vector_push_back_int( &all_degs2_below , i );
        } else {
            vector_push_back_int( &all_degs3_above , i );
            degs3_original_table.values[i].int_value = all_degs3_above.size-1;
        }
    }

    // Now we build an 'auxiliary graph' as an adjacency matrix of all deg3+.
    unsigned short **aux_adjs , **aux_dists;
    int aux_n = all_degs3_above.size;
    aux_adjs = malloc(sizeof(*aux_adjs) * aux_n );
    aux_dists = malloc(sizeof(*aux_dists) * aux_n );
    for( int i = 0 ; i < aux_n ; i++ ){
        aux_adjs[i] = malloc(sizeof(**aux_adjs) * aux_n );
        aux_dists[i] = malloc(sizeof(**aux_dists) * aux_n );
        memset(aux_adjs[i],0,aux_n);
    }
    for( int i = 0 ; i < aux_n ; i++ ){
        int original_index = vector_get_int(&all_degs3_above,i);
        vector* connected_vertex = g->adjs.values[original_index].vec_value;
        int degree = connected_vertex->size;
        for( int k = 0 ; k < degree ; k++ ){
            int original_connected_index = connected_vertex->values[k].int_value;
            int degree_connected = g->adjs.values[original_connected_index].vec_value->size;
            if( degree_connected > 2 ){
                int j = vector_get_int( &degs3_original_table , original_connected_index );
                aux_adjs[i][j] = 1;
            } else {
                // So, this deg3+ vertex is connected with a deg2- vertex. We gotta find the other deg3+ connected to this bridge and make it a edge of weight len(bridge)
                int bridge_index = g->bridge_map.values[original_connected_index].vec_value->values[0].int_value;
                bridge* b = g->bridges.values[bridge_index].ptr_value;
                int other_extremity_index = (original_index==b->extremity_0)?(b->extremity_1):(b->extremity_0);
                int j = vector_get_int( &degs3_original_table , other_extremity_index );
                aux_adjs[i][j] = (aux_adjs[i][j]==0)?(b->vertexes.size+1):(MIN( aux_adjs[i][j] , b->vertexes.size+1 ));
            }
        }
    }
    _fill_dist_matrix( aux_adjs , aux_dists , aux_n );

    printf("tcharan!\n");

    for( int i = 0 ; i < aux_n ; i++ ){
        for( int j = i ; j < aux_n ; j++ ){
            printf("%d " , aux_dists[i][j] );
        }
        printf("\n");
    }

    for( int i = 0 ; i < aux_n ; i++ ){
        free(aux_adjs[i]);
        free(aux_dists[i]);
    }
    free(aux_adjs);
    free(aux_dists);
    vector_free(&all_degs2_below);
    vector_free(&all_degs3_above);
    vector_free(&degs3_original_table);
}


int W_b( bridge *b1 ){

}
int W_bc( bridge *b1 , bridge *b2 , int d1 , int d2 ){

}
int W_bb( bridge *b1 , bridge *b2 , int da , int db , int dc , int dd ){

}

