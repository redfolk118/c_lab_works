#include "graph.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int GetVertexWithMinEdgeToMST(int vertex_count, int key_values[],
                                     bool in_mst_set[]) {
  int current_min_key_value = INFINITE_WEIGHT;
  int min_weight_vertex_index = NO_WEIGHT;
  for (int i = 0; i < vertex_count; i++) {
    if (!in_mst_set[i]) {
      if (min_weight_vertex_index == NO_WEIGHT ||
          key_values[i] < current_min_key_value) {
        current_min_key_value = key_values[i];
        min_weight_vertex_index = i;
      }
    }
  }
  return min_weight_vertex_index;
}

void PrintMstEdges(int vertex_count, int parent_nodes[]) {
  for (int i = 0; i < vertex_count; i++) {
    if (parent_nodes[i] != UNASSIGNED_PARENT) {
      int current_vertex = parent_nodes[i] + 1;
      int neighbour_vertex = i + 1;
      printf("%d %d\n",
             current_vertex < neighbour_vertex ? current_vertex
                                               : neighbour_vertex,
             current_vertex < neighbour_vertex ? neighbour_vertex
                                               : current_vertex);
    }
  }
}

void FreeGraphMatrix(int **matrix, int vertex_count) {
  if (!matrix)
    return;
  for (int i = 0; i < vertex_count; i++)
    free(matrix[i]);
  free(matrix);
}

int BuildMinimumSpanningTree(int **graph_matrix, int vertex_count,
                             int *parent_nodes) {
  int mst_edges_count = 0;
  int *key_values = malloc(vertex_count * sizeof(int));
  bool *in_mst_set = malloc(vertex_count * sizeof(bool));

  for (int i = 0; i < vertex_count; i++) {
    parent_nodes[i] = UNASSIGNED_PARENT;
    key_values[i] = INFINITE_WEIGHT;
    in_mst_set[i] = false;
  }
  key_values[0] = 0;

  for (int count = 0; count < vertex_count; count++) {
    int current_vertex =
        GetVertexWithMinEdgeToMST(vertex_count, key_values, in_mst_set);
    if (current_vertex == -1)
      break;
    in_mst_set[current_vertex] = true;
    for (int neighbour_vertex = 0; neighbour_vertex < vertex_count;
         neighbour_vertex++) {
      int edge_weight = graph_matrix[current_vertex][neighbour_vertex];
      if (edge_weight != NO_EDGE && !in_mst_set[neighbour_vertex] &&
          edge_weight <= key_values[neighbour_vertex]) {
        if (parent_nodes[neighbour_vertex] == UNASSIGNED_PARENT) {
          mst_edges_count++;
        }
        parent_nodes[neighbour_vertex] = current_vertex;
        key_values[neighbour_vertex] = edge_weight;
      }
    }
  }
  free(key_values);
  free(in_mst_set);
  return mst_edges_count;
}

ErrorCode InitializeGraph(int ***graph_matrix, int *vertex_count,
                          int *edge_count) {
  if (scanf("%d", vertex_count) != 1 || *vertex_count < 0 ||
      *vertex_count > MAX_VERTICES) {
    return ERROR_BAD_VERTICES;
  }

  if (scanf("%d", edge_count) != 1) {
    return ERROR_BAD_EDGES_COUNT;
  }

  long long max_edges =
      *vertex_count > 0 ? (long long)(*vertex_count) * (*vertex_count - 1) / 2
                        : 0;
  if (*edge_count < 0 || *edge_count > max_edges) {
    return ERROR_BAD_EDGES_COUNT;
  }

  if (*vertex_count == 0) {
    return ERROR_NO_SPANNING_TREE;
  }

  *graph_matrix = malloc(*vertex_count * sizeof(int *));

  for (int i = 0; i < *vertex_count; i++) {
    (*graph_matrix)[i] = malloc(*vertex_count * sizeof(int));
    for (int j = 0; j < *vertex_count; j++)
      (*graph_matrix)[i][j] = NO_EDGE;
  }

  for (int i = 0; i < *edge_count; i++) {
    int current_vertex, neighbour_vertex;
    long long edge_weight_long;
    if (scanf("%d %d %lld", &current_vertex, &neighbour_vertex,
              &edge_weight_long) != 3) {
      FreeGraphMatrix(*graph_matrix, *vertex_count);
      return ERROR_BAD_EDGE_LINE;
    }
    if (current_vertex < 1 || current_vertex > *vertex_count ||
        neighbour_vertex < 1 || neighbour_vertex > *vertex_count) {
      FreeGraphMatrix(*graph_matrix, *vertex_count);
      return ERROR_BAD_VERTEX;
    }
    if (edge_weight_long < 0 || edge_weight_long > INFINITE_WEIGHT) {
      FreeGraphMatrix(*graph_matrix, *vertex_count);
      return ERROR_BAD_LENGTH;
    }
    int weight = edge_weight_long;
    int from_index = current_vertex - 1, to_index = neighbour_vertex - 1;
    if ((*graph_matrix)[from_index][to_index] == NO_EDGE ||
        weight < (*graph_matrix)[from_index][to_index]) {
      (*graph_matrix)[from_index][to_index] = weight;
      (*graph_matrix)[to_index][from_index] = weight;
    }
  }
  return SUCCESS;
}
