#include "graph_utils.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 5000

Graph *CreateGraph(uint32_t vertices_count) {
  Graph *graph = malloc(sizeof(Graph));

  graph->vertices_count = vertices_count;
  graph->adjacency_matrix = malloc(vertices_count * sizeof(uint32_t *));

  uint32_t *data = malloc(vertices_count * vertices_count * sizeof(uint32_t));

  for (uint32_t i = 0; i < vertices_count; i++) {
    graph->adjacency_matrix[i] = data + i * vertices_count;
    for (uint32_t j = 0; j < vertices_count; j++) {
      graph->adjacency_matrix[i][j] = INFINITY_DISTANCE;
    }
  }
  return graph;
}

int AddEdge(Graph *graph, uint32_t current_vertex, uint32_t neighbour_vertex,
            uint32_t weight) {
  if (!graph || current_vertex >= graph->vertices_count ||
      neighbour_vertex >= graph->vertices_count)
    return -1;
  graph->adjacency_matrix[current_vertex][neighbour_vertex] = weight;
  graph->adjacency_matrix[neighbour_vertex][current_vertex] = weight;
  return 0;
}

void FreeGraph(Graph *graph) {
  if (graph) {
    free(graph->adjacency_matrix[0]);
    free(graph->adjacency_matrix);
    free(graph);
  }
}

void PrintDistances(const uint32_t *distances, const uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    if (distances[i] == INFINITY_DISTANCE)
      printf("oo ");
    else if (distances[i] > INT32_MAX)
      printf("INT_MAX+ ");
    else
      printf("%u ", distances[i]);
  }
  printf("\n");
}

static void PrintPath(const uint32_t *parents, const uint32_t start,
                      const uint32_t end) {
  if (parents[end] == INFINITY_DISTANCE && start != end) {
    printf("no path\n");
    return;
  }

  uint32_t path[MAX_VERTICES];
  int index = 0;
  for (uint32_t i = end; i != INFINITY_DISTANCE; i = parents[i]) {
    path[index++] = i + 1;
    if (i == start)
      break;
  }
  for (int i = 0; i < index; i++) {
    printf("%u ", path[i]);
  }
  printf("\n");
}

ErrorCode ReadGraph(Graph **graph_out, uint32_t *start_vertex,
                    uint32_t *end_vertex) {
  uint32_t vertices_count, start_vertex_index, end_vertex_index, edge_count;
  if (scanf("%u", &vertices_count) != 1)
    return ERROR_BAD_VERTEX_COUNT;
  if (vertices_count > MAX_VERTICES)
    return ERROR_BAD_VERTEX_COUNT;

  if (scanf("%u %u", &start_vertex_index, &end_vertex_index) != 2)
    return ERROR_BAD_VERTEX;
  if (start_vertex_index < 1 || start_vertex_index > vertices_count ||
      end_vertex_index < 1 || end_vertex_index > vertices_count)
    return ERROR_BAD_VERTEX;
  const uint32_t max_edges = vertices_count * (vertices_count - 1) / 2;
  if (scanf("%u", &edge_count) != 1)
    return ERROR_BAD_EDGE_COUNT;
  if (edge_count > max_edges)
    return ERROR_BAD_EDGE_COUNT;

  Graph *graph = CreateGraph(vertices_count);

  for (uint32_t i = 0; i < edge_count; i++) {
    uint32_t current_vertex, neighbor_vertex;
    long long edge_weight;
    if (scanf("%u %u %lld", &current_vertex, &neighbor_vertex, &edge_weight) !=
        3) {
      FreeGraph(graph);
      return ERROR_BAD_LINE;
    }
    if (current_vertex < 1 || current_vertex > vertices_count ||
        neighbor_vertex < 1 || neighbor_vertex > vertices_count) {
      FreeGraph(graph);
      return ERROR_BAD_VERTEX;
    }
    if (edge_weight < 0 || edge_weight > INT32_MAX) {
      FreeGraph(graph);
      return ERROR_BAD_LENGTH;
    }
    AddEdge(graph, current_vertex - 1, neighbor_vertex - 1, edge_weight);
  }

  *graph_out = graph;
  *start_vertex = start_vertex_index - 1;
  *end_vertex = end_vertex_index - 1;

  return ERROR_NONE;
}

void PrintPathIfAny(const uint32_t *parents, const uint32_t start,
                    const uint32_t end, const uint32_t *path_counts,
                    const uint32_t result) {
  if (result > INT_MAX && path_counts[end] >= 2) {
    printf("overflow\n");
  } else if (result == INFINITY_DISTANCE) {
    printf("no path\n");
  } else {
    PrintPath(parents, start, end);
  }
}
