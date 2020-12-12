#ifndef _LLIST_H_
#define _LLIST_H_

typedef void* data_t;

typedef void (*data_print_fn_t)(data_t data);
typedef void (*data_free_fn_t)(data_t data);
typedef struct lib_linked_list* linked_list_t;

linked_list_t list_create(data_t data, data_print_fn_t print_fn, data_free_fn_t data_free_fn);
void list_prepend(linked_list_t list, data_t data);
void list_append(linked_list_t list, data_t data);
data_t list_pop(linked_list_t list);
void list_print(linked_list_t list);
int list_isempty(linked_list_t list);
size_t list_getlen(linked_list_t list);
void list_destroy(linked_list_t list);

#endif /* _LINKED_LIST_H_ */
