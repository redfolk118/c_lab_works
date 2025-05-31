#include "dijkstra.h"
#include "graph_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  Graph *graph;
  uint32_t start, end;

  const ErrorCode errors = ReadGraph(&graph, &start, &end);
  if (errors != ERROR_NONE) {
    switch (errors) {
    case ERROR_BAD_VERTEX:
      printf("bad vertex\n");
      break;
    case ERROR_BAD_VERTEX_COUNT:
      printf("bad number of vertices\n");
      break;
    case ERROR_BAD_EDGE_COUNT:
      printf("bad number of edges\n");
      break;
    case ERROR_BAD_LENGTH:
      printf("bad length\n");
      break;
    case ERROR_BAD_LINE:
      printf("bad number of lines\n");
      break;
    default:
      break;
    }
    return 0;
  }

  uint32_t *distances = malloc(graph->vertices_count * sizeof(uint32_t));
  uint32_t *parents = malloc(graph->vertices_count * sizeof(uint32_t));
  uint32_t *path_counts = malloc(graph->vertices_count * sizeof(uint32_t));

  const uint32_t result = ComputeShortestPathAndPathCounts(
      graph, start, end, distances, parents, path_counts);

  PrintDistances(distances, graph->vertices_count);

  PrintPathIfAny(parents, start, end, path_counts, result);

  free(distances);
  free(parents);
  free(path_counts);
  FreeGraph(graph);
  return 0;
}
