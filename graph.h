#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h"

#define MIN_DIST(dist_matrix,i,j) ((i<j)?(dist_matrix[i][j]):((i>j)?(dist_matrix[j][i]):(0)))

enum TOPOLOGIES {
                                // when initializing a graph with graph_init, 'data' is a vector in which...
    TOPOLOGY_EMPTY ,            //  ... data[0] is the number of vertices, not a single edge present
    TOPOLOGY_CIRCULAR ,         //  ... data[0] is the number of vertices
    TOPOLOGY_THETA_GRAPH ,      //  ... data[0],data[1],...,data[m-1] are the amount of vertex in each of the m-bridges
    TOPOLOGY_SINGLE_BRIDGE,     //  ... data[0] is the number of vertices
    TOPOLOGY_COMPLETE_GRAPH,    //  ... data[0] is the number of vertices
    TOPOLOGY_STAR ,             //  ... data[0],data[1],...,data[m-1] are the amount of vertex in each of the m star's edges
    TOPOLOGY_TETRAHEDRON ,      //  ... data[0...5] are the amount of vertex in each of the 6 bridges
    TOPOLOGY_RANDOM ,           //  ... data[0] is the number of vertices and data[1] is the seed
};


typedef struct bridge {
    vector vertices ;
    int extremity_0;
    int extremity_1;
    int self_connected;
} bridge ;


typedef struct graph {
    vector adjs; // 'adjs' is a vector of vectors!

    unsigned int **dists; // used only in floyd-warshall alghrtm

    unsigned int **aux_dists; // dists regarding the degree>=3 vertices.
    vector bridges;     // vector of bridges 
    vector bridge_map;  // if bridge_map[i]==-1 , vertex 'i' isn't inside any bridge, else, it is inside bridge[bridge_map[i]].
    vector original_to_aux; // a single vector to map indexes from auxiliary graph to the original one
    vector aux_to_original; // a single vector to map indexes from original graph to the auxiliary one

} graph ;

static const graph empty_graph = {
    .adjs = empty_vector ,
    .dists = NULL ,
    .aux_dists = NULL ,
    .bridges = empty_vector ,
    .bridge_map = empty_vector , 
    .original_to_aux = empty_vector,
    .aux_to_original = empty_vector,
};

void graph_init( graph* g , int topology , vector* data );
void graph_clean( graph* g );
void graph_print( graph* g );
int  graph_is_connected( graph* g );
void graph_connect_vertices( graph* g , int v1 , int v2 );
void graph_connect_vertices_safe( graph* g , int v1 , int v2 );

#endif