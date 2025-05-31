#ifndef GRAHP_H
#define GRAHP_H

#include "limits.h"
#include <stdbool.h>

#define MAX_VERTICES 5000
#define INFINITE_WEIGHT INT_MAX
#define NO_EDGE -1
#define UNASSIGNED_PARENT -1
#define NO_WEIGHT -1

typedef enum {
  SUCCESS,
  ERROR_BAD_VERTICES,
  ERROR_BAD_EDGES_COUNT,
  ERROR_BAD_EDGE_LINE,
  ERROR_BAD_VERTEX,
  ERROR_BAD_LENGTH,
  ERROR_NO_SPANNING_TREE,
} ErrorCode;

ErrorCode InitializeGraph(int ***graph_matrix, int *vertex_count,
                          int *edge_count);
void FreeGraphMatrix(int **matrix, int vertex_count);
void PrintMstEdges(int vertex_count, int parent_nodes[]);
int BuildMinimumSpanningTree(int **graph_matrix, int vertex_count,
                             int *parent_nodes);
#endif // GRAHP_H
