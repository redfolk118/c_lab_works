#include "graph_units.h"
#include "topological_sort.h"
#include <stdio.h>

int main(void) {
  Graph graph = {0};
  int vertex_state[MAX_VERTICES];
  int topological_order[MAX_VERTICES];
  int order_length = 0;

  enum ErrorCode error_code = ReadGraphInput(&graph);
  if (error_code != OK) {
    switch (error_code) {
    case BAD_NUMBER_OF_LINES:
      printf("bad number of lines");
      break;
    case BAD_NUMBER_OF_VERTICES:
      printf("bad number of vertices");
      break;
    case BAD_NUMBER_OF_EDGES:
      printf("bad number of edges");
      break;
    case BAD_VERTEX:
      printf("bad vertex");
      break;
    default:
      printf("unknown error");
      break;
    }
    FreeGraph(&graph);
    return 0;
  }

  if (SortGraphTopologically(&graph, vertex_state, topological_order,
                             &order_length) != true) {
    printf("impossible to sort");
  }
  PrintTopologicalOrder(topological_order, order_length);
  FreeGraph(&graph);
  return 0;
}
