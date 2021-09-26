#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h"

enum TOPOLOGIES {
                            // when initializing a graph with graph_init, 'data' is a vector which...
    TOPOLOGY_CIRCULAR ,     //  .. data[0] is the number of vertexs
    TOPOLOGY_THETA_GRAPH    //  .. data[0],data[1],...,data[m-1] are the amount of vertex in each of the m-bridges
};


typedef struct bridge {
    vector vertexes ;
    int extremity_0;
    int extremity_1;
} bridge ;


typedef struct graph {
    vector adjs; // 'adjs' is a vector of vectors!


    // Not necessary fields below --------------

    // vector of bridges
    vector bridges; 

    // vector of vectors. 
    // if bridge_map[i] has size=1, vertex 'i' is on the bridge bridge_map[i][0]. 
    // if bridge_map[i] has size>1 , vertex 'i' connects all the bridges on bridge_map[i].
    vector bridge_map;
    
} graph ;

static const graph empty_graph = {
    .adjs = empty_vector ,
    .bridges = empty_vector ,
    .bridge_map = empty_vector
};

void graph_init( graph* g , int topology , vector* data );

#endif