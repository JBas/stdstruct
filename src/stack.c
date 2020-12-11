
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

typedef struct node lnode;
struct node {
    elem_t data;
    lnode* prev;
    lnode* next;
};

typedef struct stack lstack;
struct stack {
    size_t size;
    lnode* start;
    lnode* end;
};

lstack* stack_create() {
    dbg_printf("Creating Stack!\n");

    lstack* stack = malloc(sizeof(lstack));

    stack->start = NULL;
    stack->end = NULL;
    stack->size = 0;

    dbg_printf("Created Stack!\n");

    return stack;
}

void stack_push(lstack* stack, elem_t e) {

    if (stack == NULL) {
        dbg_printf("Error!\n");
        return;
    }

    dbg_printf("Pushing to Stack!\n");

    lnode* temp = malloc(sizeof(lnode));
    temp->prev = NULL;
    temp->next = NULL;
    temp->data = e;

    if (stack_isempty(stack)) {
        stack->start = temp;
        stack->end = temp;
    } else {
        lnode* end = stack->end;
        temp->prev = end;
        end->next = temp;

        stack->end = temp;
    }

    stack->size++;

    dbg_printf("Pushed to Stack!\n");

    return;
}

elem_t stack_pop(stacked_t stack) {
    if (stack == NULL) {
        dbg_printf("Error!\n");
        return (elem_t)-1;
    }

    if (stack_isempty(stack)) {
        dbg_printf("Stack is empty...\n");
        return (elem_t)-1;
    }

    if (stack->start == stack->end) {
        elem_t data = stack->end->data;
        free(stack->end);

        stack->start = NULL;
        stack->end = NULL;
        stack->size--;
        if (!stack_isempty(stack)) { dbg_printf("We have some issues...\n"); }
        return data;
    }

    lnode* end = stack->end;
    end->prev->next = NULL;
    stack->end = end->prev;
    end->prev = NULL;

    elem_t data = end->data;
    free(end);

    stack->size--;

    return data;
}

void stack_destroy(stacked_t stack) {
    if (stack == NULL) {
        dbg_printf("Error!\n");
        return;
    }

    lnode* n = stack->end;

    while (n != NULL) {
        lnode* temp = n->prev;
        free(n);
        n = temp;
    }

    free(stack);
}

int stack_isempty(stacked_t stack) {
    return (stack->start == NULL) && (stack->end == NULL) && (stack->size == 0);
}

size_t stack_getsize(stacked_t stack) {
    return stack->size;
}
