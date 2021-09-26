#include "list.h"

void    list_push_back( list* l , int i ){
    list_node* new_node = malloc(sizeof(list_node));
    new_node->val = i;
    new_node->next = NULL;
    if(l->first==NULL){
        l->first = new_node;
        l->last = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    } else {
        new_node->prev = l->last;
        l->last->next = new_node;
        l->last = new_node;
    }
}
int     list_pop_back( list* l ){
    int ret;
    if(l->last){
        if(l->first==l->last){
            ret = l->last->val;
            free(l->first);
            l->first = NULL;
            l->last = NULL;
        } else {
            list_node* to_pop = l->last;
            to_pop->prev->next = NULL;
            ret = l->last->val;
            l->last=to_pop->prev;
            free(to_pop);
        }
    } else ret = 0xBADC0DE;
    return ret;
}
int     list_contains( list* l , int val ){
    for( list_node* node = l->first ; node ; node = node->next ){
        if(node->val == val ) return 1;
    }
    return 0;
}
void    list_free( list* l ){
    for( list_node* node = l->first ; node ; ){
        list_node* next_node = node->next;
        free(node);
        node = next_node;
    }
}
