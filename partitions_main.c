#include "partitions.h"

void print_help(){
    printf("Usage:\n");
    printf("partitions <SUM OF PARTITIONS> <AMOUNT OF PARTITIONS>\n");
    exit(0);
}

int main( int argc , char** argv ){
    if( argc < 3 ) print_help();
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    printf("Partioning %d into %d partitions...\n" , n , m );
    print_all( n , m );
    printf("Total of %d configurations.\n" , total_partitions(n,m));

}