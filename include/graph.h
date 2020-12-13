

#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef void* vertex_t;
typedef void* edge_t;

typedef struct lib_graph* graph_t;

graph_t graph_create();
void graph_add_vertex(graph_t graph, vertex_t x);
void graph_remove_vertex(graph_t graph, vertex_t x);
void graph_add_edge(graph_t graph, vertex_t x, vertex_t y, edge_t e);
void graph_remove_edge(graph_t graph, vertex_t x, vertex_t y);
void graph_destroy();

#endif










adjacent(G, x, y): tests whether there is an edge from the vertex x to the vertex y;
neighbors(G, x): lists all vertices y such that there is an edge from the vertex x to the vertex y;
get_vertex_value(G, x): returns the value associated with the vertex x;
set_vertex_value(G, x, v): sets the value associated with the vertex x to v.
get_edge_value(G, x, y): returns the value associated with the edge (x, y);
set_edge_value(G, x, y, v): sets the value associated with the edge (x, y) to v.
