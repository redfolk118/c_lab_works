#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H
#include "graph_units.h"
#include <stdbool.h>

bool SortGraphTopologically(Graph *graph, int vertex_state[],
                            int topological_order[], int *order_length);

void PrintTopologicalOrder(const int topological_order[], int order_length);

#endif // TOPOLOGICAL_SORT_H
