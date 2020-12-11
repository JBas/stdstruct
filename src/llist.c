/** @file linked_list.c
 *  @brief A simple linked list.
 *
 *  This is the implementation of a
 *  simple linked list.
 *
 *  @author Joshua Bas (jnbas)
 *  @date March 20, 2020
 *  @bug No know bugs.
 */

#include <stdlib.h>
#include "llist.h"
#include "debug.h"

typedef struct lib_node {
    data_t data;
    struct lib_node* prev;
    struct lib_node* next;
} lnode;

typedef struct lib_linked_list {
    size_t len;
    struct lib_node* start;
    struct lib_node* end;

    data_print_fn_t print_fn;
} llist;

llist* list_create(data_t data, data_print_fn_t print_fn) {
    dbg_assert(print_fn != NULL);

    llist* list = (llist *)malloc(sizeof(llist));
    dbg_assert(list != NULL);

    list->print_fn = print_fn;
    list->start = (lnode *)malloc(sizeof(lnode));
    dbg_assert(list->start != NULL);

    list->start->data = data;
    list->start->prev = NULL;
    list->start->next = NULL;

    list->end = list->start;
    list->len = 1;
    return list;
}

void list_prepend(llist* list, data_t data) {
    dbg_assert(list != NULL);

    lnode* temp = (lnode *)malloc(sizeof(lnode));
    dbg_assert(temp != NULL);

    temp->data = data;
    temp->prev = NULL;
    temp->next = list->start;

    list->start->prev = temp;
    list->start = temp;

    list->len += 1;
    return;
}

void list_append(llist* list, data_t data) {
    dbg_assert(list != NULL);

    lnode* temp = (lnode *)malloc(sizeof(lnode));
    dbg_assert(temp != NULL);

    temp->data = data;
    temp->prev = list->end;
    temp->next = NULL;

    list->end->next = temp;
    list->end = temp;

    list->len += 1;

    return;
}

data_t list_pop(llist* list) {
    dbg_assert(list != NULL);

    lnode* temp = list->end;
    list->end = temp->prev;

    temp->prev->next = NULL;
    temp->prev = NULL;

    data_t data = temp->data;
    free(temp);

    dbg_assert(list->len > 0);
    list->len -= 1;
    return data;
}

void list_print(linked_list_t list) {
    dbg_assert(list != NULL);

    data_print_fn_t print_fn = list->print_fn;
    dbg_assert(print_fn != NULL);

    for (lnode* temp = list->start; temp != NULL; temp = temp->next) {
        print_fn(temp->data);
    }

    return;
}

int list_isempty(llist* list) {
    dbg_assert(list != NULL);
    if ((list->start == NULL) && (list->end == NULL)) {
        dbg_assert(list->len == 0);
        return 1;
    } else {
        return 0;
    }
}

size_t list_getlen(llist* list) {
    dbg_assert(list != NULL);
    return list->len;
}

void list_destroy(llist* list) {
    dbg_assert(list != NULL);

    lnode* temp = list->start;
    while (temp != NULL) {
        lnode* prev = temp;
        temp = temp->next;
        free(prev);
    }

    free(list);

    return;
}
