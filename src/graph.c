
#include <stdlib.h>
#include <math.h>
#include "hash.h"
#include "llist.h"
#include "debug.h"

/*
    [i]: (vertex, list of any (edge value, connected vertex)), ...
     .
     .
     .
*/

#define TABLE_SIZE 10

typedef struct lib_graph lgraph;
struct lib_graph {
    hash_t table;
};

/*---------------------------------------------------------------------------*/

int equiv_fn(key_t k1, key_t k2) {
    // compare addresses, so don't need to cast
    return k1 == k2;
}

int hash_fn(key_t k) {
    double ck = 0.55*(double)k;
    int hash = (int)floor(TABLE_SIZE*(ck - floor(ck)));
    return hash;
}

int print_pass(key_t k) {
    (void)k;
    return 0;
}

lgraph* graph_create() {

    lgraph* graph = (lgraph *)malloc(sizeof(lgraph));
    dbg_assert(graph != NULL);

    hash_t table = hash_create(TABLE_SIZE, &equiv_fn, &hash_fn, &print_pass);
    dbg_assert(table != NULL);

    graph->table = table;
    return graph;
}

void graph_add_vertex(lgraph* graph, vertex_t x) {
    hash_update(graph->table, (key_t)x, (val_t)NULL);
    return;
}

void graph_remove_vertex(lgraph* graph, vertex_t x) {
    hash_remove(graph->table, (key_t)x);
}

void graph_add_edge(lgraph* graph, vertex_t x, vertex_t y, edge_t e) {
    val_t v = hash_remove(graph->table, (key_t)x);
    linked_list_t conn = (linked_list_t)v;

    // @TODO: if user defines what a vertex is,
    // user needs to define how to free and how to print a vertex
    if (conn == NULL) {
        conn = list_create(data_print_fn_t print_fn, data_free_fn_t data_free_fn);
        return;
    }
    data_t* data = (data_t *)malloc(2*sizeof(data_t));
    data[0] = (data_t)e;
    data[1] = (data_t)y;
    list_append(conn, (data_t)data);
    hash_insert(graph->table, (key_t)x, (val_t)conn);
}

void graph_remove_edge(lgraph* graph, vertex_t x, vertex_t y, edge_t e) {
    val_t v = hash_remove(graph->table, (key_t)x);
    linked_list_t conn = (linked_list_t)v;

    if (conn == NULL) { return; }

    size_t len = list_getlen(conn);
    for (size_t i = 0; i < len; i++) {
        data_t* data = (data_t *)list_peek_end(conn);
        vertex_t _e = (vertex_t)(data[0]);
        vertex_t _y = (vertex_t)(data[1]);
        if (graph->table->equiv_fn(y, _y) && graph->edge_equiv_fn(e, _e)) {
            list_pop(conn);
            return;
        }
        list_cycle(conn);
    }
}

void graph_destroy(lgraph* graph) {
    hash_destroy(graph->table);
}
