#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 2000

enum ErrorCode {
  OK = 0,
  BAD_NUMBER_OF_LINES,
  BAD_NUMBER_OF_VERTICES,
  BAD_NUMBER_OF_EDGES,
  BAD_VERTEX
};

typedef struct {
  int num_vertices;
  int num_edges;
  unsigned char **adj_matrix;
} Graph;

enum ErrorCode ReadGraphInput(Graph *graph);
void FreeGraph(Graph *graph);
#endif
