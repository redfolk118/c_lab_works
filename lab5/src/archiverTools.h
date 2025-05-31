#ifndef ARCHIVERTOOLS_H
#define ARCHIVERTOOLS_H

#include "huffmanTree.h"

#include <stdbool.h>
#include <stdio.h>

void WriteMetadata(FILE *out_file, unsigned padding_bits,
                   unsigned long data_size, unsigned count);
unsigned *CreateTableOfFrequencies(FILE *in_file, unsigned long *data_size);
void WriteTableOfFrequencies(FILE *in_file, FILE *out_file,
                             unsigned *frequencies);
int CreateTableOfUsedSymbols(unsigned char *symbols, unsigned *frequencies);
void WriteLastByte(FILE *out_file, unsigned char bit_buf, unsigned bits);
void UpdatePaddingInMetadata(FILE *out_file, unsigned long data_size,
                             unsigned bits);
void EncodeAndWriteCompressedText(FILE *in_file, FILE *out_file,
                                  HuffCode *codes, unsigned long data_size,
                                  int *bits, unsigned char *bit_buf);
void ClearFile(const char *path);
bool ReadMetadata(FILE *in_file, unsigned long *data_size,
                  unsigned *padding_bits, unsigned *count);
bool ReadFrequencies(FILE *in_file, unsigned *frequencies,
                     unsigned symbols_count);
void DecodeAndWriteText(FILE *in_file, FILE *out_file, unsigned long data_size,
                        unsigned symbols_count, unsigned char *symbols,
                        HuffNode *root);
#endif // ARCHIVERTOOLS_H
