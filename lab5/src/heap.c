#include "heap.h"
#include <stdlib.h>

HuffNode *CreateNode(unsigned char symbol, unsigned frequency) {
  HuffNode *node = malloc(sizeof(HuffNode));
  node->symbol = symbol;
  node->frequency = frequency;
  node->left = NULL;
  node->right = NULL;
  return node;
}

MinHeap *CreateMinHeap(unsigned capacity) {
  MinHeap *heap = malloc(sizeof(MinHeap));
  heap->size = 0;
  heap->capacity = capacity;
  heap->nodes = malloc(capacity * sizeof(HuffNode *));
  return heap;
}

void HeapifyMin(MinHeap *heap, unsigned index) {
  unsigned smallest = index;
  unsigned left = 2 * index + 1;
  unsigned right = 2 * index + 2;

  if (left < heap->size &&
      heap->nodes[left]->frequency < heap->nodes[smallest]->frequency)
    smallest = left;
  if (right < heap->size &&
      heap->nodes[right]->frequency < heap->nodes[smallest]->frequency)
    smallest = right;

  if (smallest != index) {
    HuffNode *tmp = heap->nodes[smallest];
    heap->nodes[smallest] = heap->nodes[index];
    heap->nodes[index] = tmp;
    HeapifyMin(heap, smallest);
  }
}

HuffNode *ExtractMinNode(MinHeap *heap) {
  HuffNode *root = heap->nodes[0];
  heap->nodes[0] = heap->nodes[--heap->size];
  HeapifyMin(heap, 0);
  return root;
}

void InsertIntoMinHeap(MinHeap *heap, HuffNode *node) {
  unsigned idx = heap->size++;
  while (idx && node->frequency < heap->nodes[(idx - 1) / 2]->frequency) {
    heap->nodes[idx] = heap->nodes[(idx - 1) / 2];
    idx = (idx - 1) / 2;
  }
  heap->nodes[idx] = node;
}
