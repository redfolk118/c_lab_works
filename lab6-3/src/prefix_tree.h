#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdio.h>

#define BUFFER_SIZE 10003

typedef struct Node {
  char *label;
  char is_word;
  struct Node *children;
  struct Node *next;
} Node;

Node *CreateNode(const char *input_label, bool is_word_flag);
void InsertWord(Node *root_node, const char *input_word);
int CountNodes(const Node *root_node);
void PrintWordsWithPrefix(Node *root_node, char *input_prefix,
                          FILE *output_stream);
void FreeTree(Node *root_node);

#endif // TREE_H
