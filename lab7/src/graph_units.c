#include "graph_units.h"
#include <stdio.h>
#include <stdlib.h>

enum ErrorCode ReadGraphInput(Graph *graph) {
  if (scanf("%d", &graph->num_vertices) != 1)
    return BAD_NUMBER_OF_LINES;
  if (graph->num_vertices < 0 || graph->num_vertices > MAX_VERTICES)
    return BAD_NUMBER_OF_VERTICES;

  if (scanf("%d", &graph->num_edges) != 1)
    return BAD_NUMBER_OF_LINES;

  long long max_edges =
      (long long)graph->num_vertices * (graph->num_vertices - 1);
  if (graph->num_edges < 0 || graph->num_edges > max_edges)
    return BAD_NUMBER_OF_EDGES;

  graph->adj_matrix = malloc(graph->num_vertices * sizeof(unsigned char *));

  for (int i = 0; i < graph->num_vertices; i++) {
    graph->adj_matrix[i] = calloc(graph->num_vertices, sizeof(unsigned char));
  }

  for (int i = 0; i < graph->num_edges; i++) {
    int from, to;
    if (scanf("%d %d", &from, &to) != 2)
      return BAD_NUMBER_OF_LINES;
    if (from < 1 || from > graph->num_vertices || to < 1 ||
        to > graph->num_vertices)
      return BAD_VERTEX;
    graph->adj_matrix[from - 1][to - 1] = 1;
  }

  return OK;
}

void FreeGraph(Graph *graph) {
  if (graph->adj_matrix) {
    for (int i = 0; i < graph->num_vertices; i++) {
      free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    graph->adj_matrix = NULL;
  }
}

void PrintTopologicalOrder(const int topological_order[], int order_length) {
  for (int i = order_length - 1; i >= 0; i--) {
    printf("%d ", topological_order[i] + 1);
  }
}
