#include "partitions.h"

#define MAX_N 1000
#define MAX_M 20

int mem[MAX_N][MAX_M];


void _generate_partitions( int n , int m , vector* upper , vector* all_partitions ){
    if( m==1 ){
        vector_push_back_int(upper,n);
        vector* new_parts = malloc(sizeof(vector));
        *new_parts = empty_vector;
        for( int i = 0 ; i < upper->size ; i++ ){
            int xi = 0;
            for( int j = 0 ; j <= i ; j++ )
                xi += vector_get_int( upper , j );
            vector_push_back_int(new_parts,xi);
        }
        vector_push_back_ptr(all_partitions,new_parts);
        vector_pop_back( upper , 1 );
    } else {
        for( int i = 0 ; i <= n/m ; i+=1 ){
            vector_push_back_int( upper , i );
            _generate_partitions(n-i*m,m-1,upper,all_partitions);
            vector_pop_back( upper , 1 );
        }
    }
}
void _print_all( int n , int m , vector* upper ){
    if( m==1 ){
        vector_push_back_int(upper,n);
        for( int i = 0 ; i < upper->size ; i++ ){
            int xi = 0;
            for( int j = 0 ; j <= i ; j++ )
                xi += vector_get_int( upper , j );
            printf("%d " , xi );
        }
        printf("\n");
        vector_pop_back( upper , 1 );
    } else {
        for( int i = 0 ; i <= n/m ; i+=1 ){
            vector_push_back_int( upper , i );
            _print_all(n-i*m,m-1,upper);
            vector_pop_back( upper , 1 );
        }
    }
}

void generate_partitions( int n , int m , vector* all_partitions ){
    vector temp = empty_vector;
    _generate_partitions( n , m , &temp , all_partitions );
    vector_clean(&temp);
}
void print_all( int n , int m ){
    vector temp = empty_vector;
    _print_all( n , m , &temp );
    vector_clean(&temp);
}

int total_partitions( int n , int m ){
    if( m == 1 ) return 1;
    if( mem[n][m] ) return mem[n][m];
    int ret = 0 ;
    for( int i = 0 ; i <= n ; i+=m ) ret += total_partitions(n-i,m-1);
    mem[n][m]=ret;
    return ret;
}
