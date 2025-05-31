#ifndef KRUSKAL_H
#define KRUSKAL_H

#define PARENT_IS_ROOT -1
#define MAX_VERTEX_COUNT 5000

typedef struct {
  int vertex_start;
  int vertex_end;
  int weight;
} Edge;

int BuildMinimumSpanningTreeByKruskal(int vertex_count, int edge_count,
                                      Edge edge_list[], Edge mst_output[]);
void PrintMinimumSpanningTreeIfAny(int vertex_count, Edge mst_output[],
                                   int mst_edge_count);
#endif // KRUSKAL_H
