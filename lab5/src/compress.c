#include "compress.h"
#include "archiverTools.h"
#include "huffmanTree.h"

#include <stdlib.h>
#include <string.h>

void CompressFile(FILE *in_file, const char *out_path) {
  fseek(in_file, 1, SEEK_SET);
  unsigned long data_size = 0;
  unsigned *frequencies = CreateTableOfFrequencies(in_file, &data_size);

  FILE *out_file = fopen(out_path, "wb");
  if (data_size == 0) {
    WriteMetadata(out_file, 0, 0, 0);
    free(frequencies);
    return;
  }

  unsigned char symbols[MAX_TREE_NODES];
  unsigned count_of_used_symbols =
      CreateTableOfUsedSymbols(symbols, frequencies);
  HuffNode *root =
      BuildHuffmanTree(symbols, frequencies, count_of_used_symbols);
  HuffCode codes[MAX_TREE_NODES];

  GenerateHuffmanCodes(root, codes, 0);

  WriteMetadata(out_file, 0, data_size, count_of_used_symbols);

  WriteTableOfFrequencies(in_file, out_file, frequencies);

  unsigned char bit_buf = 0;
  int bits = 0;
  EncodeAndWriteCompressedText(in_file, out_file, codes, data_size, &bits,
                               &bit_buf);
  WriteLastByte(out_file, bit_buf, bits);
  UpdatePaddingInMetadata(out_file, data_size, bits);
  fclose(out_file);
  FreeHuffmanTree(root);
  free(frequencies);
}
