#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#define MAX_TREE_NODES 256
#define MAX_CODE_LENGTH 256
#define BITS_IN_BYTE 8

typedef struct HuffNode {
  unsigned char symbol;
  unsigned frequency;
  struct HuffNode *left;
  struct HuffNode *right;
} HuffNode;

typedef struct HuffCode {
  char bits[MAX_CODE_LENGTH];
  int length;
} HuffCode;

HuffNode *BuildHuffmanTree(unsigned char *symbols, unsigned *frequencies,
                           unsigned count);
void GenerateHuffmanCodes(HuffNode *node, HuffCode *codes, int depth);
void FreeHuffmanTree(HuffNode *node);

#endif
