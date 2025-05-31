#ifndef AVL_H
#define AVL_H
#include <stdbool.h>
#define RESULT_LIMITATION 100

typedef struct AVLNode {
  int key;
  struct AVLNode *left;
  struct AVLNode *right;
  int height;
} AVLNode;

typedef struct Rotation {
  char **rotation_mass;
  int rotation_index;
} Rotation;

int *ReadInputSequence(int *num_elements);
AVLNode *BuildAvlTree(int *input_sequence, int num_elements,
                      bool should_print_rotations, Rotation *rotations);
int GetAvlHeight(AVLNode *node);
void FreeAvlTree(AVLNode *node);

#endif
