#include "archiverTools.h"
#include "huffmanTree.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned *CreateTableOfFrequencies(FILE *in_file, unsigned long *data_size) {
  unsigned *frequencies = calloc(MAX_TREE_NODES, sizeof *frequencies);
  unsigned char byte;
  while (fread(&byte, 1, 1, in_file) == 1) {
    frequencies[byte]++;
    (*data_size)++;
  }
  return frequencies;
}

void WriteTableOfFrequencies(FILE *in_file, FILE *out_file,
                             unsigned *frequencies) {
  for (int i = 0; i < MAX_TREE_NODES; i++) {
    if (frequencies[i]) {
      unsigned char symbol = (unsigned char)i;
      fwrite(&symbol, 1, 1, out_file);
      fwrite(&frequencies[i], sizeof(frequencies[i]), 1, out_file);
    }
  }

  fseek(in_file, 1, SEEK_SET);
}

int CreateTableOfUsedSymbols(unsigned char *symbols, unsigned *frequencies) {
  int count = 0;
  for (int i = 0; i < MAX_TREE_NODES; i++) {
    if (frequencies[i])
      symbols[count++] = (unsigned char)i;
  }
  return count;
}

void WriteMetadata(FILE *out_file, unsigned padding_bits,
                   unsigned long data_size, unsigned count) {
  fwrite(&padding_bits, sizeof(padding_bits), 1, out_file);
  fwrite(&data_size, sizeof(data_size), 1, out_file);
  fwrite(&count, sizeof(count), 1, out_file);
}

void WriteLastByte(FILE *out_file, unsigned char bit_buf, unsigned bits) {
  if (!bits) {
    return;
  }
  fputc(bit_buf, out_file);
}

void UpdatePaddingInMetadata(FILE *out_file, unsigned long data_size,
                             unsigned bits) {
  if (!bits) {
    return;
  }
  unsigned padding_bits = BITS_IN_BYTE - bits;
  fseek(out_file, 0, SEEK_SET);
  fwrite(&padding_bits, sizeof(padding_bits), 1, out_file);
  fseek(out_file, sizeof(padding_bits), SEEK_SET);
  fwrite(&data_size, sizeof(data_size), 1, out_file);
}

void EncodeAndWriteCompressedText(FILE *in_file, FILE *out_file,
                                  HuffCode *codes, unsigned long data_size,
                                  int *bits, unsigned char *bit_buf) {
  unsigned char byte;
  for (unsigned long i = 0; i < data_size; i++) {
    if (fread(&byte, 1, 1, in_file) != 1) {
      fclose(out_file);
      return;
    }
    const char *code = codes[byte].bits;
    for (int j = 0; code[j]; j++) {
      if (code[j] == '1')
        *bit_buf |= 1 << (BITS_IN_BYTE - *bits - 1);
      if (++(*bits) == BITS_IN_BYTE) {
        fputc(*bit_buf, out_file);
        *bit_buf = 0;
        *bits = 0;
      }
    }
  }
}

void ClearFile(const char *path) {
  FILE *file = fopen(path, "wb");
  if (file) {
    fclose(file);
  }
}

bool ReadMetadata(FILE *in_file, unsigned long *data_size,
                  unsigned *padding_bits, unsigned *count) {
  if (fread(padding_bits, sizeof *padding_bits, 1, in_file) != 1)
    return false;

  if (fread(data_size, sizeof *data_size, 1, in_file) != 1)
    return false;

  if (fread(count, sizeof *count, 1, in_file) != 1)
    return false;

  return true;
}

bool ReadFrequencies(FILE *in_file, unsigned *frequencies,
                     unsigned symbols_count) {
  for (unsigned i = 0; i < symbols_count; i++) {
    unsigned char symbol;
    unsigned frequency;
    if (fread(&symbol, 1, 1, in_file) == 0) {
      return false;
    }
    if (fread(&frequency, sizeof(frequency), 1, in_file) == 0) {
      return false;
    }
    frequencies[symbol] = frequency;
  }
  return true;
}

void DecodeAndWriteText(FILE *in_file, FILE *out_file, unsigned long data_size,
                        unsigned symbols_count, unsigned char *symbols,
                        HuffNode *root) {
  if (symbols_count == 1) {
    for (unsigned long i = 0; i < data_size; i++)
      fputc(symbols[0], out_file);
  } else {
    int byte;
    unsigned decoded = 0;
    HuffNode *current_node = root;
    while ((byte = fgetc(in_file)) != EOF && decoded < data_size) {
      for (int b = BITS_IN_BYTE - 1; b >= 0; b--) {
        current_node =
            (byte & (1 << b)) ? current_node->right : current_node->left;
        if (!current_node->left && !current_node->right) {
          fputc(current_node->symbol, out_file);
          current_node = root;
          if (++decoded >= data_size)
            break;
        }
      }
    }
  }
}
