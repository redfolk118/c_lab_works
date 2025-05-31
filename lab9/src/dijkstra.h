#ifndef GRAPH_H
#define GRAPH_H
#include "graph_utils.h"
#include <stdint.h>

uint32_t ComputeShortestPathAndPathCounts(
    const Graph *graph, const uint32_t start_vertex, const uint32_t end_vertex,
    uint32_t *out_distances, uint32_t *out_parents, uint32_t *out_path_counts);
#endif
