#include "huffmanTree.h"
#include "heap.h"
#include <stdlib.h>
#include <string.h>

HuffNode *BuildHuffmanTree(unsigned char *symbols, unsigned *frequencies,
                           unsigned count) {
  MinHeap *heap = CreateMinHeap(count);
  for (unsigned i = 0; i < count; ++i)
    heap->nodes[i] = CreateNode(symbols[i], frequencies[symbols[i]]);
  heap->size = count;
  for (int i = (heap->size - 1) / 2; i >= 0; --i)
    HeapifyMin(heap, i);
  while (heap->size > 1) {
    HuffNode *left = ExtractMinNode(heap);
    HuffNode *right = ExtractMinNode(heap);
    HuffNode *parent = CreateNode('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    InsertIntoMinHeap(heap, parent);
  }
  HuffNode *root = ExtractMinNode(heap);
  free(heap->nodes);
  free(heap);
  return root;
}

static void CreateHuffmanCodeTable(HuffNode *node, HuffCode *codes,
                                   char *buffer, int depth) {
  if (!node->left && !node->right) {
    if (depth == 0)
      buffer[depth++] = '0';
    buffer[depth] = '\0';
    strcpy(codes[node->symbol].bits, buffer);
    codes[node->symbol].length = depth;
    return;
  }
  if (node->left) {
    buffer[depth] = '0';
    CreateHuffmanCodeTable(node->left, codes, buffer, depth + 1);
  }
  if (node->right) {
    buffer[depth] = '1';
    CreateHuffmanCodeTable(node->right, codes, buffer, depth + 1);
  }
}

void GenerateHuffmanCodes(HuffNode *node, HuffCode *codes, int depth) {
  memset(codes, 0, sizeof(*codes));
  char buffer[MAX_CODE_LENGTH];
  CreateHuffmanCodeTable(node, codes, buffer, depth);
}

void FreeHuffmanTree(HuffNode *node) {
  if (!node)
    return;
  FreeHuffmanTree(node->left);
  FreeHuffmanTree(node->right);
  free(node);
}
