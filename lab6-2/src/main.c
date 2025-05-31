#include "BTree.h"
#include <stdio.h>

int main() {
  int degree, num_keys;
  if (scanf("%d %d", &degree, &num_keys) != 2)
    return 1;

  BTree tree = {.degree = degree, .root_node = NULL};

  for (int i = 0; i < num_keys; i++) {
    int data;
    if (scanf("%d", &data) != 1)
      return 1;
    InsertKeyIntoTree(&tree, data);
  }

  printf("%d\n", CalculateTreeHeight(&tree));
  FreeBTree(&tree);
  return 0;
}
