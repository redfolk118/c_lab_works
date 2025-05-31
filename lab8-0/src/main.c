#include "Kruskal.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int vertex_count = 0;
  if (scanf("%d", &vertex_count) != 1) {
    return 0;
  }
  if (vertex_count > MAX_VERTEX_COUNT) {
    printf("bad number of vertices\n");
    return 0;
  }

  int edge_count = 0;
  if (scanf("%d", &edge_count) != 1) {
    return 0;
  }
  const long long max_possible_edges =
      (long long)vertex_count * (vertex_count - 1) / 2;
  if (edge_count > max_possible_edges) {
    printf("bad number of edges\n");
    return 0;
  }
  if (vertex_count == 0 && edge_count == 0) {
    printf("no spanning tree\n");
    return 0;
  }

  Edge *edge_list = malloc(sizeof(Edge) * edge_count);

  for (int edge_index = 0; edge_index < edge_count; edge_index++) {
    long long input_weight = 0;
    int input_vertex_start = 0;
    int input_vertex_end = 0;

    if (scanf("%d %d %lld", &input_vertex_start, &input_vertex_end,
              &input_weight) != 3) {
      printf("bad number of lines\n");
      free(edge_list);
      return 0;
    }
    if (input_vertex_start < 1 || input_vertex_start > vertex_count ||
        input_vertex_end < 1 || input_vertex_end > vertex_count) {
      printf("bad vertex\n");
      free(edge_list);
      return 0;
    }
    if (input_weight < 0 || input_weight > INT_MAX) {
      printf("bad length\n");
      free(edge_list);
      return 0;
    }

    edge_list[edge_index].vertex_start = input_vertex_start - 1;
    edge_list[edge_index].vertex_end = input_vertex_end - 1;
    edge_list[edge_index].weight = input_weight;
  }

  Edge *mst_output = malloc(sizeof(Edge) * (vertex_count - 1));

  int mst_edge_count = BuildMinimumSpanningTreeByKruskal(
      vertex_count, edge_count, edge_list, mst_output);

  PrintMinimumSpanningTreeIfAny(vertex_count, mst_output, mst_edge_count);

  free(edge_list);
  free(mst_output);
  return 0;
}
