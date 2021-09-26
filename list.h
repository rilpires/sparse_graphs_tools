#ifndef LIST_H
#define LIST_H

#include <stdlib.h>



// Not circular
typedef struct list_node {
    int val;
    struct list_node* next;
    struct list_node* prev;
} list_node ;

typedef struct list {
    struct list_node* first;
    struct list_node* last;
} list;

static const list empty_list = {
    .first = NULL,
    .last = NULL
};

void    list_push_back( list* l , int i );
int     list_pop_back( list* l );
int     list_contains( list* l , int val );
void    list_free( list* l );


#endif