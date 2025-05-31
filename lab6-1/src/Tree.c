#include "Tree.h"
#include <stdint.h>

uint32_t TreeCreateNode(Node *nodes_pool, int *next_free_index,
                        int max_node_count, int node_key) {
  if (*next_free_index >= max_node_count)
    return NIL_INDEX;
  uint32_t idx = (*next_free_index)++;
  nodes_pool[idx].value = node_key;
  nodes_pool[idx].color = RED_COLOR;
  nodes_pool[idx].parent_index = NIL_INDEX;
  nodes_pool[idx].left_index = NIL_INDEX;
  nodes_pool[idx].right_index = NIL_INDEX;
  return idx;
}

void TreeRotateLeft(Node *nodes_pool, uint32_t *root_ptr,
                    uint32_t rotate_index) {
  uint32_t right_child_index = nodes_pool[rotate_index].right_index;
  nodes_pool[rotate_index].right_index =
      nodes_pool[right_child_index].left_index;
  if (nodes_pool[right_child_index].left_index != NIL_INDEX)
    nodes_pool[nodes_pool[right_child_index].left_index].parent_index =
        rotate_index;

  nodes_pool[right_child_index].parent_index =
      nodes_pool[rotate_index].parent_index;
  if (nodes_pool[rotate_index].parent_index == NIL_INDEX) {
    *root_ptr = right_child_index;
  } else {
    uint32_t parent_index = nodes_pool[rotate_index].parent_index;
    if (rotate_index == nodes_pool[parent_index].left_index)
      nodes_pool[parent_index].left_index = right_child_index;
    else
      nodes_pool[parent_index].right_index = right_child_index;
  }

  nodes_pool[right_child_index].left_index = rotate_index;
  nodes_pool[rotate_index].parent_index = right_child_index;
}

void TreeRotateRight(Node *nodes_pool, uint32_t *root_ptr,
                     uint32_t rotate_index) {
  uint32_t left_child_index = nodes_pool[rotate_index].left_index;
  nodes_pool[rotate_index].left_index =
      nodes_pool[left_child_index].right_index;
  if (nodes_pool[left_child_index].right_index != NIL_INDEX)
    nodes_pool[nodes_pool[left_child_index].right_index].parent_index =
        rotate_index;

  nodes_pool[left_child_index].parent_index =
      nodes_pool[rotate_index].parent_index;
  if (nodes_pool[rotate_index].parent_index == NIL_INDEX) {
    *root_ptr = left_child_index;
  } else {
    uint32_t parent_index = nodes_pool[rotate_index].parent_index;
    if (rotate_index == nodes_pool[parent_index].right_index)
      nodes_pool[parent_index].right_index = left_child_index;
    else
      nodes_pool[parent_index].left_index = left_child_index;
  }

  nodes_pool[left_child_index].right_index = rotate_index;
  nodes_pool[rotate_index].parent_index = left_child_index;
}

int CalculateBlackHeight(Node *nodes_pool, uint32_t start_index) {
  int black_height = 0;
  while (start_index != NIL_INDEX) {
    if (nodes_pool[start_index].color == BLACK_COLOR)
      black_height++;
    start_index = nodes_pool[start_index].left_index;
  }
  return black_height + 1;
}

static void BalanceTreeAfterInsertion(Node *nodes_pool, uint32_t *root_ptr,
                                      uint32_t inserted_index) {
  while (inserted_index != *root_ptr &&
         nodes_pool[nodes_pool[inserted_index].parent_index].color ==
             RED_COLOR) {
    uint32_t parent = nodes_pool[inserted_index].parent_index;
    uint32_t grandparent = nodes_pool[parent].parent_index;
    if (parent == nodes_pool[grandparent].left_index) {
      uint32_t uncle = nodes_pool[grandparent].right_index;
      if (uncle != NIL_INDEX && nodes_pool[uncle].color == RED_COLOR) {
        nodes_pool[parent].color = BLACK_COLOR;
        nodes_pool[uncle].color = BLACK_COLOR;
        nodes_pool[grandparent].color = RED_COLOR;
        inserted_index = grandparent;
      } else {
        if (inserted_index == nodes_pool[parent].right_index) {
          inserted_index = parent;
          TreeRotateLeft(nodes_pool, root_ptr, inserted_index);
          parent = nodes_pool[inserted_index].parent_index;
          grandparent = nodes_pool[parent].parent_index;
        }
        nodes_pool[parent].color = BLACK_COLOR;
        nodes_pool[grandparent].color = RED_COLOR;
        TreeRotateRight(nodes_pool, root_ptr, grandparent);
      }
    } else {
      uint32_t uncle = nodes_pool[grandparent].left_index;
      if (uncle != NIL_INDEX && nodes_pool[uncle].color == RED_COLOR) {
        nodes_pool[parent].color = BLACK_COLOR;
        nodes_pool[uncle].color = BLACK_COLOR;
        nodes_pool[grandparent].color = RED_COLOR;
        inserted_index = grandparent;
      } else {
        if (inserted_index == nodes_pool[parent].left_index) {
          inserted_index = parent;
          TreeRotateRight(nodes_pool, root_ptr, inserted_index);
          parent = nodes_pool[inserted_index].parent_index;
          grandparent = nodes_pool[parent].parent_index;
        }
        nodes_pool[parent].color = BLACK_COLOR;
        nodes_pool[grandparent].color = RED_COLOR;
        TreeRotateLeft(nodes_pool, root_ptr, grandparent);
      }
    }
  }
  nodes_pool[*root_ptr].color = BLACK_COLOR;
}

void TreeInsertNode(Node *nodes_pool, int *next_free_index, int max_node_count,
                    uint32_t *root_index_ptr, int node_key) {
  uint32_t new_index =
      TreeCreateNode(nodes_pool, next_free_index, max_node_count, node_key);
  if (new_index == NIL_INDEX)
    return;
  uint32_t parent = NIL_INDEX;
  uint32_t current = *root_index_ptr;
  while (current != NIL_INDEX) {
    parent = current;
    if (node_key < nodes_pool[current].value)
      current = nodes_pool[current].left_index;
    else
      current = nodes_pool[current].right_index;
  }
  nodes_pool[new_index].parent_index = parent;
  if (parent == NIL_INDEX)
    *root_index_ptr = new_index;
  else if (node_key < nodes_pool[parent].value)
    nodes_pool[parent].left_index = new_index;
  else
    nodes_pool[parent].right_index = new_index;
  BalanceTreeAfterInsertion(nodes_pool, root_index_ptr, new_index);
}
