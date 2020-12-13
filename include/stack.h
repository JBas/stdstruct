

#ifndef _STACK_H_
#define _STACK_H_

typedef void* elem_t;
typedef struct lib_stack* stacked_t;

stacked_t stack_create();
int stack_isempty(stacked_t stack);
size_t stack_getsize(stacked_t stack);
void stack_push(stacked_t stack, elem_t e);
elem_t stack_pop(stacked_t stack);
void stack_destroy();

#endif /* _STACK_H_ */
