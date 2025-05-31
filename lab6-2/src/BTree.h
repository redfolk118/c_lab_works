#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>

typedef struct BTreeNode {
  int key_count;
  int *keys;
  struct BTreeNode **children;
  bool is_leaf;
} BTreeNode;

typedef struct {
  int degree;
  BTreeNode *root_node;
} BTree;

void InsertKeyIntoTree(BTree *tree, int new_key);
int CalculateTreeHeight(BTree *tree);
void FreeBTree(BTree *tree);

#endif // BTREE_H
