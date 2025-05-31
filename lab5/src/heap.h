#ifndef HEAP_H
#define HEAP_H

#include "huffmanTree.h"

typedef struct MinHeap {
  unsigned size;
  unsigned capacity;
  HuffNode **nodes;
} MinHeap;

HuffNode *CreateNode(unsigned char symbol, unsigned frequency);
MinHeap *CreateMinHeap(unsigned capacity);
void HeapifyMin(MinHeap *heap, unsigned index);
HuffNode *ExtractMinNode(MinHeap *heap);
void InsertIntoMinHeap(MinHeap *heap, HuffNode *node);

#endif
