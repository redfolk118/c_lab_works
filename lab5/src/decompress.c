#include "decompress.h"
#include "archiverTools.h"
#include "huffmanTree.h"
#include <stdbool.h>
#include <stdio.h>

void DecompressFile(const char *in_path, const char *out_path) {
  FILE *in_file = fopen(in_path, "rb");

  unsigned long data_size;
  unsigned padding_bits, count;

  if (ReadMetadata(in_file, &data_size, &padding_bits, &count) == false) {
    fclose(in_file);
    return;
  }

  if (data_size == 0) {
    fclose(in_file);
    ClearFile(out_path);
    return;
  }

  unsigned frequencies[MAX_TREE_NODES] = {0};
  if (ReadFrequencies(in_file, frequencies, count) == false) {
    fclose(in_file);
    return;
  }

  unsigned char symbols[MAX_TREE_NODES];
  const unsigned symbols_count = CreateTableOfUsedSymbols(symbols, frequencies);

  HuffNode *root = BuildHuffmanTree(symbols, frequencies, symbols_count);
  FILE *out_file = fopen(out_path, "wb");

  DecodeAndWriteText(in_file, out_file, data_size, symbols_count, symbols,
                     root);
  ClearFile(in_path);
  fclose(in_file);
  fclose(out_file);
  FreeHuffmanTree(root);
}
