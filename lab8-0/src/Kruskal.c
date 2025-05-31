#include "Kruskal.h"

#include <stdio.h>
#include <stdlib.h>

static int CompareEdgesByWeight(const void *first_ptr, const void *second_ptr) {
  const Edge *first_edge = first_ptr;
  const Edge *second_edge = second_ptr;
  if (first_edge->weight < second_edge->weight)
    return -1;
  if (first_edge->weight > second_edge->weight)
    return 1;
  return 0;
}

int FindSetRoot(int vertex_index, int parent_array[]) {
  if (parent_array[vertex_index] < 0) {
    return vertex_index;
  }
  parent_array[vertex_index] =
      FindSetRoot(parent_array[vertex_index], parent_array);
  return parent_array[vertex_index];
}

void UnionSetsBySize(int first_root, int second_root, int parent_array[]) {
  first_root = FindSetRoot(first_root, parent_array);
  second_root = FindSetRoot(second_root, parent_array);
  if (first_root == second_root) {
    return;
  }
  if (parent_array[first_root] > parent_array[second_root]) {
    int temporary_root = first_root;
    first_root = second_root;
    second_root = temporary_root;
  }
  parent_array[first_root] += parent_array[second_root];
  parent_array[second_root] = first_root;
}

int BuildMinimumSpanningTreeByKruskal(int vertex_count, int edge_count,
                                      Edge edge_list[], Edge mst_output[]) {
  qsort(edge_list, edge_count, sizeof(Edge), CompareEdgesByWeight);

  int *parent_array = malloc(sizeof(int) * vertex_count);
  for (int i = 0; i < vertex_count; i++) {
    parent_array[i] = PARENT_IS_ROOT;
  }

  int edges_added_to_mst = 0;
  for (int edge_index = 0;
       edge_index < edge_count && edges_added_to_mst < vertex_count - 1;
       edge_index++) {
    int start_vertex = edge_list[edge_index].vertex_start;
    int end_vertex = edge_list[edge_index].vertex_end;

    int root_start = FindSetRoot(start_vertex, parent_array);
    int root_end = FindSetRoot(end_vertex, parent_array);

    if (root_start != root_end) {
      UnionSetsBySize(root_start, root_end, parent_array);
      mst_output[edges_added_to_mst++] = edge_list[edge_index];
    }
  }

  free(parent_array);
  return edges_added_to_mst;
}

void PrintMinimumSpanningTreeIfAny(int vertex_count, Edge mst_output[],
                                   int mst_edge_count) {
  if (mst_edge_count < vertex_count - 1 && vertex_count > 0) {
    printf("no spanning tree\n");
    return;
  }

  for (int output_index = 0; output_index < mst_edge_count; output_index++) {
    printf("%d %d\n", mst_output[output_index].vertex_start + 1,
           mst_output[output_index].vertex_end + 1);
  }
}
