#ifndef GRAPHUNITS_H
#define GRAPHUNITS_H
#include <stdint.h>

#define INFINITY_DISTANCE UINT32_MAX

typedef struct {
  uint32_t **adjacency_matrix;
  uint32_t vertices_count;
} Graph;

typedef enum {
  ERROR_NONE,
  ERROR_BAD_VERTEX,
  ERROR_BAD_VERTEX_COUNT,
  ERROR_BAD_EDGE_COUNT,
  ERROR_BAD_LENGTH,
  ERROR_BAD_LINE,
  ERROR_NO_PATH,
  ERROR_OVERFLOW
} ErrorCode;

ErrorCode ReadGraph(Graph **graph_out, uint32_t *start_vertex,
                    uint32_t *end_vertex);
void FreeGraph(Graph *graph);
void PrintDistances(const uint32_t *distances, const uint32_t count);
void PrintPathIfAny(const uint32_t *parents, const uint32_t start,
                    const uint32_t end, const uint32_t *path_counts,
                    const uint32_t result);
#endif
