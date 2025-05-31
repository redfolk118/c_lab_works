#include "graph.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int vertex_count, edge_count;
  int **graph_matrix = NULL;

  ErrorCode error_code =
      InitializeGraph(&graph_matrix, &vertex_count, &edge_count);
  if (error_code != SUCCESS) {
    switch (error_code) {
    case ERROR_BAD_VERTICES:
      printf("bad number of vertices\n");
      break;
    case ERROR_BAD_EDGES_COUNT:
      printf("bad number of edges\n");
      break;
    case ERROR_BAD_EDGE_LINE:
      printf("bad number of lines\n");
      break;
    case ERROR_BAD_VERTEX:
      printf("bad vertex\n");
      break;
    case ERROR_BAD_LENGTH:
      printf("bad length\n");
      break;
    case ERROR_NO_SPANNING_TREE:
      printf("no spanning tree\n");
      break;
    default:
      break;
    }
    return 0;
  }

  int *parent_nodes = malloc(vertex_count * sizeof(int));

  int mst_edges_count =
      BuildMinimumSpanningTree(graph_matrix, vertex_count, parent_nodes);

  if (vertex_count > 1 && mst_edges_count < vertex_count - 1) {
    printf("no spanning tree\n");
  } else {
    PrintMstEdges(vertex_count, parent_nodes);
  }

  FreeGraphMatrix(graph_matrix, vertex_count);
  free(parent_nodes);
  return 0;
}
