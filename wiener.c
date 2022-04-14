#include <math.h>
#include <stdio.h>
#include <string.h>

#include "wiener.h"

void _fill_dist_matrix_floyd_warshall( unsigned int** dists , int n ){
    
    for( int k = 0 ; k < n ; k++ ){
        for( int i = 0 ; i < n ; i++ ){
            for( int j = i+1 ; j < n ; j++ ){

                unsigned int dist_ki = (k<i) ? (dists[k][i]) : (dists[i][k]) ;
                unsigned int dist_kj = (k<j) ? (dists[k][j]) : (dists[j][k]) ;
                if( dist_ki==0 || dist_kj==0 ){
                    continue;
                } else if( dists[i][j] == 0 ){
                    dists[i][j] = dist_ki + dist_kj ;
                    dists[j][i] = dists[i][j];
                } else {
                    dists[i][j] = MIN( dists[i][j] , dist_ki + dist_kj ) ;
                    dists[j][i] = dists[i][j];
                }
            }
        }   
    }
}


void _fill_dist_matrix_dijkstra( unsigned int** dists , vector* adjs ){
    
    int n = adjs->size;

    for( int i = 0 ; i < n ; i++ ){
        // Applying dijkstra here with vertex 'i' as source
    
        vector *i_adj_vector = adjs->values[i].vec_value;
        list discovery_queue = empty_list;
        vector visiteds = empty_vector;
        vector_resize( &visiteds , n );
        vector_int_fill( &visiteds , 0 );
        vector_set_int( &visiteds , i , 1 );

        for( int i_adj_index = 0 ; i_adj_index < i_adj_vector->size ; i_adj_index++ ){
            int j = vector_get_int( i_adj_vector, i_adj_index );
            list_push_back( &discovery_queue , j );
            vector_set_int( &visiteds , j , 1 );
        }

        while( discovery_queue.first ){
            
            int j = discovery_queue.first->val;
            vector *j_adj_vector = adjs->values[j].vec_value;
            list_pop_front( &discovery_queue );

            for( int j_adj_index = 0 ; j_adj_index < j_adj_vector->size ; j_adj_index++ ){
                int k = vector_get_int(j_adj_vector,j_adj_index);
                unsigned int old_distance = ((i<k)?(dists[i][k]):(dists[k][i]));
                unsigned int new_distance = ((i<j)?(dists[i][j]):(dists[j][i])) +   
                                            ((j<k)?(dists[j][k]):(dists[k][j]));
                
                
                if( (new_distance<old_distance)||( (i!=k) && (old_distance==0)) ) {
                    if(i<k) dists[i][k] = new_distance;
                    else    dists[k][i] = new_distance;
                }

                if( vector_get_int( &visiteds , k )==0 ){
                    list_push_back( &discovery_queue , k );
                    vector_set_int( &visiteds , k , 1 );
                }
                
            }

            
        }

        list_clean( &discovery_queue );
        vector_clean( &visiteds );

    }

}


void _fill_bridges( graph *g ){

    int n = g->adjs.size;
    int m = 0 ; // total edge count
    
    vector_clean(&(g->bridge_map));
    vector_resize(&g->bridge_map,n);
    for( int i = 0 ; i < n ; i++ ){
        vector_set_int( &g->bridge_map , i , -1 );
        m += g->adjs.values[i].vec_value->size;
    }
    m /= 2;
    //printf("m=%f n\n" , ((float)m)/n );
    vector_clean(&(g->bridges));

    vector_resize( &g->original_to_aux , n );
    for( int i = 0 ; i < n ; i++ ){
        if( g->adjs.values[i].vec_value->size > 2 ){
            vector_set_int( &g->original_to_aux , i , g->aux_to_original.size );
            vector_push_back_int( &g->aux_to_original , i );
        }
    }

    int aux_n = g->aux_to_original.size;
    
    vector visiteds = empty_vector;
    vector_resize( &visiteds , n );
    
    for( int i = 0 ; i < n ; i++ ){
        if( vector_get_int( &visiteds , i ) == 0 ){
            vector_set_int( &visiteds , i , 1 );
            int degree = g->adjs.values[i].vec_value->size;
            int bridge_index = g->bridge_map.values[i].int_value;
            if( degree <= 2 && bridge_index == -1 ){
                bridge* new_bridge = malloc(sizeof(bridge));
                new_bridge->extremity_0 = -1;
                new_bridge->extremity_1 = -1;
                new_bridge->self_connected = 0;
                new_bridge->vertices = empty_vector;
                bridge_index = g->bridges.size;
                vector_push_back_ptr( &g->bridges , new_bridge );

                list bridge_discovery = empty_list;
                list_push_back( &bridge_discovery , i );
                while( bridge_discovery.last ){
                    int j = list_pop_back(&bridge_discovery);
                    int j_degree = g->adjs.values[j].vec_value->size;
                    vector_push_back_int( &new_bridge->vertices , j );
                    vector_set_int( &g->bridge_map , j , bridge_index );

                    for( int ji = 0 ; ji < j_degree ; ji++ ){
                        int k = g->adjs.values[j].vec_value->values[ji].int_value;
                        int k_degree = g->adjs.values[k].vec_value->size;
                        if( k_degree <= 2 ){
                            if( vector_get_int( &visiteds , k )==0 ){
                                vector_set_int( &visiteds , k , 1 );
                                list_push_back( &bridge_discovery , k );
                            }
                        } else {
                            if( new_bridge->extremity_0 == -1 ) new_bridge->extremity_0 = k;
                            else if( new_bridge->extremity_1 == -1 ) new_bridge->extremity_1 = k;
                            else printf("wtf?");
                        }
                    }
                }
                list_clean(&bridge_discovery);
            }
        }
    }
    vector_clean( &visiteds );

    // This is the unique case where m = n and there is only one bridge: it is a ring graph.
    if( m == n && g->bridges.size == 1 ){
        bridge* b = g->bridges.values[0].ptr_value;
        b->self_connected = 1;
    }

}


// sum from a single bridge
int _W_b( bridge *b1 , unsigned int d ){

    int n = b1->vertices.size;
    int ret;

    int k = (n-1+d)/2;
    k = CLAMP(k,1,n-1);
    int p1 = (n*k*(k+1))/2;
    int p2 = ( n*n - n + d*n )*( n - k - 1);
    int p3 = ((1-d-2*n)*(n+k)*(n-k-1))/2;
    int p4 = SQUARED_SUM(n-1) -2*SQUARED_SUM(k);
    ret = p1 + p2 + p3 + p4;
    
    return ret;
}
// sum between a bridge and a central
int _W_bc( bridge *b1 , unsigned int d1 , unsigned int d2 ){
    int ret = 0;
    int n = b1->vertices.size;
    int dx1 = MIN( d1 , n-1+d2);
    int dx2 = MIN( d2 , n-1+d1);
    int a = MIN(dx1,dx2);
    int b = MAX(dx1,dx2);
    int i = (n-ABS(a-b))/2;
    int na = n - i;
    int nb = n - na;
    ret = 2*a*na + na*na - na;
    ret += 2*b*nb + nb*nb - nb;
    ret /= 2;
    return ret;
}
// sum between 2 bridges
int _W_bb( bridge *b1 , bridge *b2 , unsigned int da , unsigned int db , unsigned int dc , unsigned int dd ){
    if( b1->vertices.size < b2->vertices.size ){
        return _W_bb(b2,b1,da,dc,db,dd); // when flipping, swap db<=>dc
    }

    int ret = 0;
    int n1 = b1->vertices.size;
    int n2 = b2->vertices.size;
    for( int i = 0 ; i < n2 ; i++ ){
        int d1 = MIN( da + i , dc + n2 - 1 - i );
        int d2 = MIN( db + i , dd + n2 - 1 - i );
        ret += _W_bc( b1 , d1 , d2 );
    }
    return ret;
}


int W( graph *g , enum WIENER_METHOD which_method ){
    int ret;
    switch( which_method ){
        case WIENER_METHOD_FLOYD_WARSHALL:
        case WIENER_METHOD_MULTIPLE_DIJKSTRAS:
            ret = W_traditional(g,which_method);
            break;
        case WIENER_METHOD_SPARSE_METHOD:
        default:
            ret = W_sparse(g);
            break;
    }
    //printf("dist matrix:\n");
    //int n = g->adjs.size;
    //for( int i = 0 ; i < n ; i++ ){
    //    for( int j = 0 ; j < n ; j++ ){
    //        printf("%d " , g->dists[i][j] );
    //    }
    //    printf("\n");
    //}
    return ret;
}

int W_traditional( graph *g , enum WIENER_METHOD which_method ){
    
    int n = g->adjs.size;
    
    if( g->dists ){
        for( int i = 0 ; i < n ; i++ ){
            free( g->dists[i] );
        }
        free( g->dists );
    }

    g->dists = malloc( sizeof(unsigned int*) * n );
    for( int i = 0 ; i < n ; i++ ){
        g->dists[i] = malloc( sizeof(unsigned int) * n );
        memset(g->dists[i] , 0 , sizeof(unsigned int) * n );
    }

    for( int i = 0 ; i < n ; i++ ){
        for( int k = 0 ; k < g->adjs.values[i].vec_value->size ; k++ ){
            int j = g->adjs.values[i].vec_value->values[k].int_value;
            g->dists[i][j] = 1;
        }
    }

    switch (which_method)
    {
    case WIENER_METHOD_FLOYD_WARSHALL:
        _fill_dist_matrix_floyd_warshall( g->dists , n );
        break;
    case WIENER_METHOD_MULTIPLE_DIJKSTRAS:
        _fill_dist_matrix_dijkstra( g->dists , &(g->adjs) );
        break;
    default:
        printf("ERROR - this case isn't supposed to happen");
        break;
    }
    
    
    int ret = 0 ;
    for( int i = 0 ; i < n ; i++ ){
        for( int j = i+1 ; j < n ; j++ ){
            if( g->dists[i][j] > 10 ){
                printf("achei um");
            }
            ret += g->dists[i][j];
        }
    }
    return 2*ret;
}

int W_sparse( graph *g ){
    int ret = 0;
    int n = g->adjs.size;

    _fill_bridges(g);

    // Now we build an 'auxiliary graph' as an adjacency matrix of all deg3+.

    vector_resize(&g->original_to_aux , n);
    vector_clean(&g->aux_to_original);
    for( int i = 0 ; i < n ; i++ ){
        if( g->adjs.values[i].vec_value->size > 2 ){
            vector_push_back_int( &g->aux_to_original , i );
            g->original_to_aux.values[i].int_value = g->aux_to_original.size-1;
        }
    }

    int aux_n = g->aux_to_original.size;
    g->aux_dists = malloc(sizeof(*g->aux_dists) * aux_n );
    for( int i = 0 ; i < aux_n ; i++ ){
        g->aux_dists[i] = malloc(sizeof(**g->aux_dists) * aux_n );
        memset(g->aux_dists[i],0,sizeof(g->aux_dists[0][0])*aux_n);
        g->aux_dists[i][i] = 0;
    }
    for( int aux_i = 0 ; aux_i < aux_n ; aux_i++ ){
        int i = vector_get_int(&g->aux_to_original,aux_i);
        int i_degree = g->adjs.values[i].vec_value->size;
        for( int ii = 0 ; ii < i_degree ; ii++ ){
            int j = g->adjs.values[i].vec_value->values[ii].int_value;
            int j_degree = g->adjs.values[j].vec_value->size;
            // This deg3+ vertex is connected with this another deg3+ vertex
            if( j_degree > 2 ){
                int aux_j = vector_get_int( &g->original_to_aux , j );
                g->aux_dists[aux_i][aux_j] = 1;
                g->aux_dists[aux_j][aux_i] = 1;
            } 
            // So, this deg3+ vertex is connected with a deg2- vertex. We gotta find, if exists, the other deg3+ connected to this bridge and make it a edge of weight len(bridge) for the aux graph.
            else {
                int bridge_index = g->bridge_map.values[j].int_value;
                bridge* b = g->bridges.values[bridge_index].ptr_value;
                int other_extremity_index = (i==b->extremity_0)?(b->extremity_1):(b->extremity_0);
                if( other_extremity_index != -1 ){
                    int aux_j = vector_get_int( &g->original_to_aux , other_extremity_index );
                    unsigned int current_dist = MIN_DIST(g->aux_dists,aux_i,aux_j);
                    g->aux_dists[aux_i][aux_j] = (current_dist==0)?
                                                    (b->vertices.size+1):(MIN( current_dist , b->vertices.size+1 ));
                    g->aux_dists[aux_j][aux_i] = g->aux_dists[aux_i][aux_j];
                }
            }
        }
    }
    
    _fill_dist_matrix_floyd_warshall( g->aux_dists , aux_n );

    for( int i = 0 ; i < aux_n ; i++ )
    for( int j = i+1 ; j < aux_n ; j++ ){
        ret += 2*MIN_DIST(g->aux_dists,i,j);
    }

    for( int bi = 0 ; bi < g->bridges.size ; bi++ ){
        bridge* br = g->bridges.values[bi].ptr_value;

        int d;

        // Unique case where the whole graph is circular
        if( br->self_connected ){ 
            d = 1;
        } 
        
        // Cases where there is only one extremity connected
        else if( br->extremity_0==-1 || br->extremity_1==-1 ){
            for( int aux_i = 0 ; aux_i < aux_n ; aux_i++ ){
                int available_extremity = (br->extremity_1==-1)?(br->extremity_0):(br->extremity_0);
                int aux_available_extremity = vector_get_int( &g->original_to_aux , available_extremity );
                int d1 = 1+MIN_DIST(g->aux_dists,aux_available_extremity,aux_i);
                int d2 = d1 + br->vertices.size + 2 ;
                ret += 2*_W_bc( br , d1 , d2 );
            }
            d = n+1; // d=n+1 guarantees that it wont be considered later on
        } else {
            int aux_extremity_0 = g->original_to_aux.values[br->extremity_0].int_value;
            int aux_extremity_1 = g->original_to_aux.values[br->extremity_1].int_value;    
            for( int i = 0 ; i < aux_n ; i++ ){
                int d1 = 1+MIN_DIST(g->aux_dists,aux_extremity_0,i);
                int d2 = 1+MIN_DIST(g->aux_dists,aux_extremity_1,i);
                ret += 2*_W_bc( br , d1 , d2 );
            }
            d = 2+MIN_DIST(g->aux_dists,aux_extremity_0,aux_extremity_1);
        }
        
        ret += 2*_W_b( br , d );
    }
    for( int b1 = 0 ; b1 < g->bridges.size ; b1++ ){
        for( int b2 = b1+1 ; b2 < g->bridges.size ; b2++ ){
            bridge* br1 = g->bridges.values[b1].ptr_value;
            bridge* br2 = g->bridges.values[b2].ptr_value;

            int extremity_00 = br1->extremity_0;
            int extremity_01 = br1->extremity_1;
            int extremity_10 = br2->extremity_0;
            int extremity_11 = br2->extremity_1;

            int aux_extremity_00 = (extremity_00==-1)?(-1):(g->original_to_aux.values[extremity_00].int_value);
            int aux_extremity_01 = (extremity_01==-1)?(-1):(g->original_to_aux.values[extremity_01].int_value);
            int aux_extremity_10 = (extremity_10==-1)?(-1):(g->original_to_aux.values[extremity_10].int_value);
            int aux_extremity_11 = (extremity_11==-1)?(-1):(g->original_to_aux.values[extremity_11].int_value);

            int da = (extremity_00 == -1||extremity_10 == -1) ? (n+2) : (2+MIN_DIST( g->aux_dists , aux_extremity_00 , aux_extremity_10 ));
            int db = (extremity_01 == -1||extremity_10 == -1) ? (n+2) : (2+MIN_DIST( g->aux_dists , aux_extremity_01 , aux_extremity_10 ));
            int dc = (extremity_00 == -1||extremity_11 == -1) ? (n+2) : (2+MIN_DIST( g->aux_dists , aux_extremity_00 , aux_extremity_11 ));
            int dd = (extremity_01 == -1||extremity_11 == -1) ? (n+2) : (2+MIN_DIST( g->aux_dists , aux_extremity_01 , aux_extremity_11 ));
            ret += 2*_W_bb( br1 , br2 , da , db , dc , dd );
        }
    }

    return ret;
}
