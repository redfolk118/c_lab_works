#include "graph_units.h"

#include <stdbool.h>

enum Color { WHITE = 0, GRAY = 1, BLACK = 2 };

static void ExecuteDepthFirstSearch(Graph *graph, int current_vertex,
                                    int vertex_state[], int topological_order[],
                                    int *order_length,
                                    bool *cycle_not_detected) {
  vertex_state[current_vertex] = GRAY;
  for (int neighbour = 0; neighbour < graph->num_vertices; neighbour++) {
    if (!graph->adj_matrix[current_vertex][neighbour]) {
      continue;
    }
    if (vertex_state[neighbour] == WHITE) {
      ExecuteDepthFirstSearch(graph, neighbour, vertex_state, topological_order,
                              order_length, cycle_not_detected);
      if (!*cycle_not_detected)
        return;
    } else if (vertex_state[neighbour] == GRAY) {
      *cycle_not_detected = false;
      return;
    }
  }
  vertex_state[current_vertex] = BLACK;
  topological_order[(*order_length)++] = current_vertex;
}

bool SortGraphTopologically(Graph *graph, int vertex_state[],
                            int topological_order[], int *order_length) {
  for (int i = 0; i < graph->num_vertices; i++) {
    vertex_state[i] = WHITE;
  }
  bool cycle_not_detected = true;
  for (int v = 0; v < graph->num_vertices; v++) {
    if (vertex_state[v] == WHITE) {
      ExecuteDepthFirstSearch(graph, v, vertex_state, topological_order,
                              order_length, &cycle_not_detected);
    }
  }
  return cycle_not_detected;
}
