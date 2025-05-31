#include "dijkstra.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define VISITED true
#define UNVISITED false

uint32_t ComputeShortestPathAndPathCounts(
    const Graph *graph, const uint32_t start_vertex, const uint32_t end_vertex,
    uint32_t *out_distances, uint32_t *out_parents, uint32_t *out_path_counts) {
  bool *visited = malloc(graph->vertices_count * sizeof(bool));

  for (uint32_t i = 0; i < graph->vertices_count; i++) {
    out_distances[i] = INFINITY_DISTANCE;
    out_parents[i] = INFINITY_DISTANCE;
    out_path_counts[i] = 0;
    visited[i] = UNVISITED;
  }
  out_distances[start_vertex] = 0;
  out_path_counts[start_vertex] = 1;

  for (uint32_t j = 0; j < graph->vertices_count; j++) {
    uint32_t min_distance = INFINITY_DISTANCE;
    uint32_t node_index_with_min_distance = INFINITY_DISTANCE;
    for (uint32_t i = 0; i < graph->vertices_count; i++) {
      if (visited[i] == UNVISITED && out_distances[i] < min_distance) {
        min_distance = out_distances[i];
        node_index_with_min_distance = i;
      }
    }
    if (node_index_with_min_distance == INFINITY_DISTANCE)
      break;
    visited[node_index_with_min_distance] = VISITED;

    for (uint32_t i = 0; i < graph->vertices_count; i++) {
      uint32_t edge_weight =
          graph->adjacency_matrix[node_index_with_min_distance][i];
      if (edge_weight != INFINITY_DISTANCE && visited[i] == UNVISITED) {
        uint32_t new_distance =
            out_distances[node_index_with_min_distance] + edge_weight;
        if (new_distance < out_distances[i]) {
          out_distances[i] = new_distance;
          out_parents[i] = node_index_with_min_distance;
          out_path_counts[i] = out_path_counts[node_index_with_min_distance];
        } else if (new_distance == out_distances[i]) {
          out_path_counts[i] += out_path_counts[node_index_with_min_distance];
        }
      }
    }
  }

  free(visited);
  return out_distances[end_vertex];
}
