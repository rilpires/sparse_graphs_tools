#include "vector.h"


void _vector_push_back( vector* v ){
    if( v->size >= v->capacity ){
        if(v->capacity==0) v->capacity = 1;
        else v->capacity = 2 * v->capacity;
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


void vector_pop_back( vector* v , int amount ){
    if( v->size <= 0 ) return;
    if( amount > v->size ) amount = v->size;
    v->size -= amount ;
    if( (v->size>0) && (v->capacity/v->size >= 2) ){

        v->capacity = v->capacity/2;
        if(v->capacity > 0 ){
            vector_value* old_values = v->values;
            vector_value* new_values = malloc(sizeof(v->values[0])*(v->capacity) );
            for( int i = 0 ; i < v->size ; i++ )
                new_values[i] = old_values[i];
            v->values = new_values;
            free(old_values);
        } else {
            free(v->values);
        }

    } else if (v->size==0){
        v->capacity = 0;
        free(v->values);
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

void vector_free( vector* v ){
    if(v){
        v->size = 0;
        v->capacity = 0;
        if(v->values)free(v->values);
    }
}