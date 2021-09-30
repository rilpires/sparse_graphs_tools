#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

#define MAX_N 1000
#define MAX_M 20

int mem[MAX_N][MAX_M];


void print_all( int n , int m , vector* upper ){
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
            print_all(n-i*m,m-1,upper);
            vector_pop_back( upper , 1 );
        }
    }
}
int total_partitions( int n , int m ){
    if( m == 1 ) return 1;
    if( mem[n][m] ) return mem[n][m];
    int ret = 0 ;
    for( int i = 0 ; i <= n ; i+=m ) ret += total_partitions(n-i,m-1);
    mem[n][m]=ret;
    return ret;
}

void print_help(){
    printf("Usage:\n");
    printf("partitions <SUM OF PARTITIONS> <AMOUNT OF PARTITIONS>\n");
    exit(0);
}
int main( int argc , char** argv ){
    if( argc < 3 ) print_help();
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    vector v = empty_vector;

    printf("Partioning %d into %d partitions...\n" , n , m );
    print_all( n , m , &v );
    printf("Total of %d configurations.\n" , total_partitions(n,m));

}