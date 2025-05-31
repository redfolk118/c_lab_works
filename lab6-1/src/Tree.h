#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdint.h>

#define RED_COLOR 0
#define BLACK_COLOR 1
#define NIL_INDEX UINT32_MAX

typedef struct Node {
  int value;
  int color;
  uint32_t parent_index;
  uint32_t left_index;
  uint32_t right_index;
} Node;

void TreeInsertNode(Node *nodes_pool, int *next_free_index, int max_node_count,
                    uint32_t *root_index_ptr, int node_key);
int CalculateBlackHeight(Node *nodes_pool, uint32_t start_index);

#endif
