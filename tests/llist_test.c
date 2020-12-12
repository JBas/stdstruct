
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "llist.h"

void print_fn(data_t data) {
    printf("%c\n", (char)data);
}

int main() {
    linked_list_t list = list_create((data_t)'J', &print_fn);

    list_append(list, (data_t)'B');
    list_append(list, (data_t)'a');
    list_append(list, (data_t)'s');
    list_prepend(list, (data_t)'@');
    list_prepend(list, (data_t)'*');
    list_prepend(list, (data_t)'\\');
    list_prepend(list, (data_t)'w');

    list_print(list);

    list_pop(list);

    list_print(list);

    list_destroy(list);

}
