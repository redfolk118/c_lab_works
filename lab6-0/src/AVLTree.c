#include "AVLTree.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_ELEMENTS 2000000
#define MIN_NUM_ELEMENTS 0

int GetAvlHeight(AVLNode *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

int GetHeightDifference(AVLNode *node) {
  if (node == NULL)
    return 0;
  return GetAvlHeight(node->left) - GetAvlHeight(node->right);
}

void UpdateHeight(AVLNode *node) {
  int left_height = GetAvlHeight(node->left);
  int right_height = GetAvlHeight(node->right);
  if (left_height > right_height)
    node->height = 1 + left_height;
  else
    node->height = 1 + right_height;
}

AVLNode *CreateNewNode(int key) {
  AVLNode *new_node = malloc(sizeof(AVLNode));
  new_node->key = key;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->height = 1;
  return new_node;
}

AVLNode *DoRightRotate(AVLNode *node) {
  AVLNode *left_child = node->left;
  AVLNode *temp_subtree = left_child->right;
  left_child->right = node;
  node->left = temp_subtree;
  UpdateHeight(node);
  UpdateHeight(left_child);
  return left_child;
}

AVLNode *DoLeftRotate(AVLNode *node) {
  AVLNode *right_child = node->right;
  AVLNode *temp_subtree = right_child->left;
  right_child->left = node;
  node->right = temp_subtree;
  UpdateHeight(node);
  UpdateHeight(right_child);
  return right_child;
}

AVLNode *DoBigRightRotate(AVLNode *node) {
  node->left = DoLeftRotate(node->left);
  return DoRightRotate(node);
}

AVLNode *DoBigLeftRotate(AVLNode *node) {
  node->right = DoRightRotate(node->right);
  return DoLeftRotate(node);
}

AVLNode *InsertToTheAVLTree(AVLNode *current_node, int new_key,
                            bool should_print_rotations, Rotation *rotations) {
  if (current_node == NULL)
    return CreateNewNode(new_key);

  if (new_key < current_node->key)
    current_node->left = InsertToTheAVLTree(current_node->left, new_key,
                                            should_print_rotations, rotations);
  else if (new_key > current_node->key)
    current_node->right = InsertToTheAVLTree(current_node->right, new_key,
                                             should_print_rotations, rotations);
  else
    return current_node;

  UpdateHeight(current_node);
  int const height_difference = GetHeightDifference(current_node);

  if (height_difference > 1) {
    if (new_key < current_node->left->key) {
      if(should_print_rotations == true) {
        rotations->rotation_mass[rotations->rotation_index] = "SR";
        rotations->rotation_index++;
      }
      return DoRightRotate(current_node);
    }
    if (new_key > current_node->left->key) {
      if(should_print_rotations == true) {
        rotations->rotation_mass[rotations->rotation_index] = "BR";
        rotations->rotation_index++;
      }
      return DoBigRightRotate(current_node);
    }
  }
  if (height_difference < -1) {
    if (new_key > current_node->right->key) {
      if(should_print_rotations == true) {
        rotations->rotation_mass[rotations->rotation_index] = "SL";
        rotations->rotation_index++;
      }
      return DoLeftRotate(current_node);
    }
    if (new_key < current_node->right->key) {
      if(should_print_rotations == true) {
        rotations->rotation_mass[rotations->rotation_index] = "BL";
        rotations->rotation_index++;
      }
      return DoBigLeftRotate(current_node);
    }
  }
  return current_node;
}

int *ReadInputSequence(int *num_elements) {
  if (scanf("%d", num_elements) != 1)
    return NULL;
  if (*num_elements < MIN_NUM_ELEMENTS || *num_elements > MAX_NUM_ELEMENTS) {
    printf("Bad input");
    return NULL;
  }
  int *input_sequence = malloc((*num_elements) * sizeof(int));
  for (int i = 0; i < *num_elements; i++) {
    if (scanf("%d", &input_sequence[i]) != 1) {
      free(input_sequence);
      return NULL;
    }
  }
  return input_sequence;
}

AVLNode *BuildAvlTree(int *input_sequence, int num_elements,
                      bool should_print_rotations, Rotation *rotations) {
  AVLNode *avl_tree_root = NULL;
  for (int i = 0; i < num_elements; i++) {
    avl_tree_root = InsertToTheAVLTree(avl_tree_root, input_sequence[i],
                                       should_print_rotations, rotations);
  }
  return avl_tree_root;
}

void FreeAvlTree(AVLNode *node) {
  if (node == NULL)
    return;
  FreeAvlTree(node->left);
  FreeAvlTree(node->right);
  free(node);
}
