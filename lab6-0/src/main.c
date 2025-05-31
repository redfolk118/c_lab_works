#include "AVLTree.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int num_elements;
  int *input_sequence = ReadInputSequence(&num_elements);
  if (input_sequence == NULL)
    return 1;

  bool should_print_rotations = false;
  Rotation *rotations = malloc(sizeof(Rotation));
  rotations->rotation_mass = malloc(num_elements * sizeof(char *));

  rotations->rotation_index = 0;

  if (num_elements < RESULT_LIMITATION) {
    should_print_rotations = true;
  }

  AVLNode *avl_tree_root = BuildAvlTree(input_sequence, num_elements,
                                        should_print_rotations, rotations);

  if (should_print_rotations) {
    for (int i = 0; i < rotations->rotation_index; i++) {
      printf("%s\n", rotations->rotation_mass[i]);
    }
  }
  int tree_height = GetAvlHeight(avl_tree_root);
  printf("%d\n", tree_height);

  free(input_sequence);
  free(rotations->rotation_mass);
  free(rotations);

  FreeAvlTree(avl_tree_root);
  return 0;
}
