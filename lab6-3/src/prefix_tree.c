#include "prefix_tree.h"
#include "string_units.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *CreateNode(const char *input_label, bool is_word_flag) {
  Node *new_node = malloc(sizeof(Node));
  if (input_label != NULL) {
    new_node->label = DuplicateString(input_label);
  } else {
    new_node->label = NULL;
  }
  new_node->is_word = is_word_flag;
  new_node->children = NULL;
  new_node->next = NULL;
  return new_node;
}

static int ComputeCommonPrefixLength(const char *first_string,
                                     const char *second_string) {
  int prefix_length = 0;
  while (first_string[prefix_length] && second_string[prefix_length] &&
         first_string[prefix_length] == second_string[prefix_length]) {
    prefix_length++;
  }
  return prefix_length;
}

void InsertWord(Node *root_node, const char *input_word) {
  for (Node *child_node = root_node->children; child_node != NULL;
       child_node = child_node->next) {
    int common_length =
        ComputeCommonPrefixLength(child_node->label, input_word);
    if (common_length == 0) {
      continue;
    }
    int label_length = strlen(child_node->label);
    if (common_length < label_length) {
      Node *suffix_node =
          CreateNode(child_node->label + common_length, child_node->is_word);
      suffix_node->children = child_node->children;
      free(child_node->label);
      child_node->label = CopyString(input_word, common_length);
      child_node->is_word = false;
      child_node->children = suffix_node;
    }
    if ((int)strlen(input_word) > common_length) {
      InsertWord(child_node, input_word + common_length);
    } else {
      child_node->is_word = true;
    }
    return;
  }

  Node *new_child_node = CreateNode(input_word, true);
  new_child_node->next = root_node->children;
  root_node->children = new_child_node;
}

static int CountNodesImpl(const Node *root_node) {
  if (root_node == NULL) {
    return 0;
  }
  int node_count = 1;
  for (const Node *child_node = root_node->children; child_node != NULL;
       child_node = child_node->next) {
    node_count += CountNodesImpl(child_node);
  }
  return node_count;
}

int CountNodes(const Node *root_node) {
  return CountNodesImpl(root_node) - (root_node->children->next == NULL);
}

static void PrintLeafNodes(const Node *current_node, char *output_buffer,
                           int *buffer_position, FILE *output_stream) {
  if (current_node->label != NULL) {
    int label_length = strlen(current_node->label);
    memcpy(output_buffer + *buffer_position, current_node->label, label_length);
    *buffer_position += label_length;
  }
  if (current_node->is_word) {
    output_buffer[*buffer_position] = '\0';
    fprintf(output_stream, "%s ", output_buffer);
  }
  for (const Node *child_node = current_node->children; child_node != NULL;
       child_node = child_node->next) {
    PrintLeafNodes(child_node, output_buffer, buffer_position, output_stream);
  }
  if (current_node->label != NULL) {
    *buffer_position -= (int)strlen(current_node->label);
  }
}

void PrintWordsWithPrefix(Node *root_node, char *input_prefix,
                          FILE *output_stream) {
  Node *current_node = root_node;
  input_prefix[strcspn(input_prefix, "\n")] = '\0';
  char prefix_buffer[BUFFER_SIZE];
  int buffer_position = 0;
  const char *remaining_prefix = input_prefix;

  while (*remaining_prefix) {
    Node *matching_node = NULL;
    for (Node *child_node = current_node->children; child_node != NULL;
         child_node = child_node->next) {
      int common_length =
          ComputeCommonPrefixLength(child_node->label, remaining_prefix);
      if (common_length > 0) {
        matching_node = child_node;
        int fragment_length = (int)strlen(child_node->label);
        memcpy(prefix_buffer + buffer_position, child_node->label,
               fragment_length);
        buffer_position += fragment_length;
        remaining_prefix += common_length;
        break;
      }
    }
    if (matching_node == NULL) {
      fprintf(output_stream, "None");
      return;
    }
    current_node = matching_node;
  }

  buffer_position -= (int)strlen(current_node->label);
  PrintLeafNodes(current_node, prefix_buffer, &buffer_position, output_stream);
}

void FreeTree(Node *root_node) {
  if (root_node == NULL) {
    return;
  }
  free(root_node->label);
  for (Node *child_node = root_node->children; child_node != NULL;) {
    Node *next_sibling = child_node->next;
    FreeTree(child_node);
    child_node = next_sibling;
  }
  free(root_node);
}
