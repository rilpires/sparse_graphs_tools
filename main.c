#include <stdio.h>

#define MAX_PARTS 500
#define MAX_SUM 500

int count( int sum , int entities ){
    if(entities==0) return 0;
    if(entities==1) return 1;
    if(sum<=1) return 1;
    
    static int mem_count[MAX_SUM][MAX_PARTS];

    if( mem_count[sum][entities] == 0 ){
        int ret = 0;
        int mod = sum % entities;
        int i;
        for( i = 0 ; i*entities + mod <= sum ; i++ )
            ret += count( i*entities + mod , entities-1 );
        mem_count[sum][entities] = ret;
    }

    return mem_count[sum][entities];
}

int count_g( int n , int m ){
    int paths = (m-n) + 2;
    int ret = count(n-2,paths) - count(n-2,paths-2) ;
    return ret;
}

int main(){
    int sum,entities;
    
    for( entities=2 ; entities<5 ; entities++ ){
        for( sum=0; sum<50 ; sum++ ){
            printf("f(\t%d,\t%d ) =\t%d\n" , sum , entities , count(sum,entities) );
        }
    }
    
    printf("\n\n");
    int delta = 100;
    for( int n=5 ; n<200 ; n++ ){
        printf("count_g( n = %d,\tm = %d )\t=\t%d\n" , n , n+delta , count_g(n,n+delta) );
    }
    

}