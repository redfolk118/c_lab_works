#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int total_nodes_count, input_key;
  if (scanf("%d", &total_nodes_count) != 1)
    return 1;
  if (total_nodes_count == 0) {
    printf("0");
    return 0;
  }
  Node *nodes_pool = malloc(sizeof(Node) * total_nodes_count);
  int next_free_index = 0;
  uint32_t root_index = NIL_INDEX;
  int max_node_count = total_nodes_count;
  next_free_index = 0;
  for (int i = 0; i < total_nodes_count; i++) {
    if (scanf("%d", &input_key) != 1)
      break;
    TreeInsertNode(nodes_pool, &next_free_index, max_node_count, &root_index,
                   input_key);
  }
  const int black_height = CalculateBlackHeight(nodes_pool, root_index);
  printf("%d", black_height);
  free(nodes_pool);
  return 0;
}
