#include "BTree.h"

#include <stdlib.h>
#include <string.h>

static BTreeNode *CreateNewNode(bool is_leaf, int minimum_degree) {
  BTreeNode *new_node = malloc(sizeof(BTreeNode));
  new_node->is_leaf = is_leaf;
  new_node->key_count = 0;
  new_node->keys = malloc((2 * minimum_degree - 1) * sizeof(int));
  new_node->children = malloc((2 * minimum_degree) * sizeof(BTreeNode *));
  return new_node;
}

static void SplitChildNode(BTree *tree, BTreeNode *parent_node,
                           int child_index) {
  int tree_order = tree->degree;
  BTreeNode *child_node = parent_node->children[child_index];
  BTreeNode *new_node = CreateNewNode(child_node->is_leaf, tree_order);
  new_node->key_count = tree_order - 1;

  for (int key_index = 0; key_index < tree_order - 1; key_index++) {
    new_node->keys[key_index] = child_node->keys[key_index + tree_order];
  }

  if (!child_node->is_leaf) {
    for (int child_pos = 0; child_pos < tree_order; child_pos++) {
      new_node->children[child_pos] =
          child_node->children[child_pos + tree_order];
    }
  }

  child_node->key_count = tree_order - 1;

  for (int shift_index = parent_node->key_count; shift_index >= child_index + 1;
       shift_index--) {
    parent_node->children[shift_index + 1] = parent_node->children[shift_index];
  }

  parent_node->children[child_index + 1] = new_node;

  for (int shift_index = parent_node->key_count - 1; shift_index >= child_index;
       shift_index--) {
    parent_node->keys[shift_index + 1] = parent_node->keys[shift_index];
  }

  parent_node->keys[child_index] = child_node->keys[tree_order - 1];
  parent_node->key_count++;
}

static int FindInsertionIndexForKey(const int *keys, int key_count,
                                    int key_to_insert) {
  int search_start = 0;
  int search_end = key_count;

  while (search_start < search_end) {
    int probe_index = search_start + (search_end - search_start) / 2;
    if (keys[probe_index] < key_to_insert)
      search_start = probe_index + 1;
    else
      search_end = probe_index;
  }

  return search_start;
}

void InsertKeyIntoTree(BTree *tree, int new_key) {
  int tree_order = tree->degree;
  if (tree->root_node == NULL) {
    tree->root_node = CreateNewNode(true, tree_order);
    tree->root_node->keys[0] = new_key;
    tree->root_node->key_count = 1;
    return;
  }

  if (tree->root_node->key_count == 2 * tree_order - 1) {
    BTreeNode *old_root = tree->root_node;
    tree->root_node = CreateNewNode(false, tree_order);
    tree->root_node->children[0] = old_root;
    SplitChildNode(tree, tree->root_node, 0);
  }

  BTreeNode *current_node = tree->root_node;
  while (!current_node->is_leaf) {
    int insert_pos = FindInsertionIndexForKey(current_node->keys,
                                              current_node->key_count, new_key);
    if (current_node->children[insert_pos]->key_count == 2 * tree_order - 1) {
      SplitChildNode(tree, current_node, insert_pos);
      if (new_key > current_node->keys[insert_pos])
        insert_pos++;
    }
    current_node = current_node->children[insert_pos];
  }

  int insertion_index = FindInsertionIndexForKey(
      current_node->keys, current_node->key_count, new_key);
  memmove(&current_node->keys[insertion_index + 1],
          &current_node->keys[insertion_index],
          (current_node->key_count - insertion_index) * sizeof(int));
  current_node->keys[insertion_index] = new_key;
  current_node->key_count++;
}

int CalculateTreeHeight(BTree *tree) {
  int height = 0;
  BTreeNode *current_node = tree->root_node;
  while (current_node) {
    height++;
    if (current_node->is_leaf)
      break;
    current_node = current_node->children[0];
  }
  return height;
}

static void FreeTreeNode(BTreeNode *node, int degree) {
  if (!node->is_leaf) {
    for (int child_index = 0; child_index <= node->key_count; child_index++) {
      FreeTreeNode(node->children[child_index], degree);
    }
  }
  free(node->keys);
  free(node->children);
  free(node);
}

void FreeBTree(BTree *tree) {
  if (tree->root_node)
    FreeTreeNode(tree->root_node, tree->degree);
}
