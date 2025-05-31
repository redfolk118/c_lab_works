#include "compress.h"
#include "decompress.h"
#include <stdio.h>

int main() {
  FILE *input_file = fopen("in.txt", "rb");

  int mode = fgetc(input_file);
  if (mode == 'c') {
    CompressFile(input_file, "compressed.bin");
  }
  if (mode == 'd') {
    DecompressFile("compressed.bin", "out.txt");
  }

  fclose(input_file);
  return 0;
}
