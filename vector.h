#ifndef VECTOR_H
#define VECTOR_H


#include <stdlib.h>
#include <string.h>


#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))
#define CLAMP(x,min,max) (MIN(max,MAX(x,min)))
#define ABS(a) (((a)>0)?((a)):(-(a)))
#define SQUARED_SUM(N) (((N)*((N)+1)*(2*(N)+1))/6)

struct vector;

// vectors can hold multiple types of value:
typedef union vector_value {
    int int_value;
    void* ptr_value;
    struct vector* vec_value;
} vector_value;

typedef struct vector {
    vector_value* values;
    unsigned int size;
    unsigned int capacity;
} vector ;


static const vector_value empty_value = {0};
static const vector empty_vector = { 
    .values = NULL , 
    .size = 0 , 
    .capacity = 0 
};

void    vector_push_back_int( vector* v , int new_int );
int     vector_get_int( vector* v , int index );
void    vector_set_int( vector* v , int index , int new_val );

void    vector_push_back_ptr( vector* v , void* new_ptr );
void*   vector_get_ptr( vector* v , int index );
void    vector_set_ptr( vector* v , int index , void* new_val );

int     vector_compare( vector* v1 , vector* v2 );
int     vector_contains( vector* v , int val );
void    vector_pop_back( vector* v , int amount );
void    vector_resize( vector* v , int new_size );
void    vector_clean( vector* v );

#endif