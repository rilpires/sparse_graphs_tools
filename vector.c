#include "vector.h"


void _vector_push_back( vector* v ){
    if( v->size >= v->capacity ){
        v->capacity = MAX(1,2*v->capacity);
        vector_value* old_values = v->values;
        vector_value* new_values = malloc(sizeof(v->values[0])*(v->capacity) );
        if( old_values ){
            for( int i = 0 ; i < v->size ; i++ )
                new_values[i] = old_values[i];
            free(old_values);
        }
        v->values = new_values;
    }
    v->size += 1;
}

void vector_push_back_int( vector* v , int new_int ){
    _vector_push_back(v);
    v->values[v->size-1].int_value = new_int;
}
int     vector_get_int( vector* v , int index ){
    return v->values[index].int_value;
}
void    vector_set_int( vector* v , int index , int new_val ){
    v->values[index].int_value = new_val;
}
void    vector_int_fill( vector* v , int value ){
    for( int i = 0 ; i < v->size ; i++ ){
        v->values[i].int_value = value;
    }
}

void vector_push_back_ptr( vector* v , void* new_ptr ){
    _vector_push_back(v);
    v->values[v->size-1].ptr_value = new_ptr;
}
void*   vector_get_ptr( vector* v , int index ){
    return v->values[index].ptr_value;
}
void    vector_set_ptr( vector* v , int index , void* new_val ){
    v->values[index].ptr_value = new_val;
}

int     vector_contains( vector* v , int val ){
    for( int i = 0 ; i < v->size ; i++ )
        if( (v->values[i]).int_value == val ) 
            return 1;
    return 0;
}

void vector_pop_back( vector* v , int amount ){
    if( v->size <= 0 ) return;
    if( amount > v->size ) amount = v->size;
    v->size -= amount ;
    if( (v->size>0) && (v->capacity/v->size >= 2) ){

        while(v->capacity > 2*v->size) 
            v->capacity /= 2;
        
        vector_value* old_values = v->values;
        vector_value* new_values = malloc(sizeof(v->values[0])*(v->capacity) );
        for( int i = 0 ; i < v->size ; i++ )
            new_values[i] = old_values[i];
        v->values = new_values;
        free(old_values);

    } else if (v->size==0){
        v->capacity = 0;
        if(v->values) free(v->values);
        v->values = NULL;
    }
}

void vector_resize( vector* v , int new_size ){
    if( new_size < 0 ) return;
    if( new_size == v->size ) return;

    int old_size = v->size;
    int new_capacity = 1;
    while(new_capacity<new_size)new_capacity*=2;
    
    v->size = new_size;
    v->capacity = new_capacity;

    vector_value* old_values = v->values;
    vector_value* new_values = malloc(sizeof(vector_value)*new_capacity);
    if( new_size > old_size ){
        memcpy(new_values,old_values,sizeof(vector_value)*old_size);
        for( int i = old_size ; i < new_size ; i++ ) new_values[i] = empty_value;
        free(old_values);
        v->values = new_values;
    } else if (new_size < old_size) {
        memcpy(new_values,old_values,sizeof(vector_value)*new_size);
        free(old_values);
        v->values = new_values;
    }
}


int vector_compare( vector* v1 , vector* v2 ){
    if( v1 == v2 ) return 0;
    if( v1 == NULL || v2 == NULL ) return 1;
    if( v1->size != v2->size ) return 1;
    for( int i = 0 ; i < v1->size ; i++ ){
        if( vector_get_int(v1,i) != vector_get_int(v2,i) ) return 1;
    }
    return 0;
}

void vector_clean( vector* v ){
    if(v){
        if(v->values)free(v->values);
        *v = empty_vector;
    }
}