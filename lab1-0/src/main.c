#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERN_LENGTH 17
#define MAX_TEXT_LENGTH 1024
#define ALPHABET_SIZE 256

void CreateStopSymbolTable(const char *pattern, int pattern_len,
                           int stop_symbol_table[]) {
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    stop_symbol_table[i] = pattern_len;
  }

  for (int i = 0; i < pattern_len - 1; i++) {
    unsigned char symbol = pattern[i];
    stop_symbol_table[(int)symbol] = pattern_len - i - 1;
  }
}

bool LoadNextChunk(int shift, int pattern_len, char *text, int *text_len,
                   int *start_index, FILE *file) {
  if (shift + *start_index < *text_len) {
    return true;
  }

  int remaining = *text_len - *start_index;

  memmove(text, text + *start_index, remaining);

  int bytes_read =
      fread(text + remaining, sizeof(char), MAX_TEXT_LENGTH - remaining, file);
  *text_len = remaining + bytes_read;
  *start_index = 0;

  return *text_len >= pattern_len;
}

void CheckLastTextSymbol(char *text, int *text_len, FILE *file) {
  if (*text_len > 0 && text[0] == '\n') {
    memmove(text, text + 1, *text_len - 1);
    (*text_len)--;

    int bytes_read = fread(text + *text_len, sizeof(char), 1, file);
    if (bytes_read == 1) {
      (*text_len)++;
    }
  }
}

void CheckLastPatternSymbol(char *pattern, int *pattern_len) {
  if (pattern[*pattern_len - 1] == '\n' && *pattern_len > 0) {
    pattern[*pattern_len - 1] = 0;
    (*pattern_len)--;
  }
}

void ComparePatterAndTextIndex(char *text, const char *pattern, int pattern_len,
                               int text_end_idx, int sample_end_idx,
                               int position) {
  for (int i = 0; i < pattern_len; ++i) {
    printf("%d ", position - i);
    if (pattern[sample_end_idx - i] != text[text_end_idx - i]) {
      break;
    }
  }
}

void FindPatternByBoyerMoore(FILE *file, const char *pattern, int pattern_len) {
  int stop_symbol_table[ALPHABET_SIZE] = {0};
  CreateStopSymbolTable(pattern, pattern_len, stop_symbol_table);

  char *text = (char *)malloc(MAX_TEXT_LENGTH * sizeof(char));
  int text_len = fread(text, sizeof(char), MAX_TEXT_LENGTH, file);

  CheckLastTextSymbol(text, &text_len, file);

  if (text_len < pattern_len) {
    free(text);
    return;
  }

  int start_index = pattern_len - 1;
  int position = pattern_len;

  while (start_index < text_len) {
    int text_end_idx = start_index;
    int sample_end_idx = pattern_len - 1;

    ComparePatterAndTextIndex(text, pattern, pattern_len, text_end_idx,
                              sample_end_idx, position);

    const int shift = stop_symbol_table[(unsigned char)text[text_end_idx]];

    if (!LoadNextChunk(shift, pattern_len, text, &text_len, &start_index,
                       file) ||
        shift == 0) {
      break;
    }
    start_index += shift;
    position += shift;
  }
  free(text);
}

int main() {
  FILE *file = fopen("in.txt", "r");
  char pattern[MAX_PATTERN_LENGTH];
  if (fgets(pattern, MAX_PATTERN_LENGTH, file) == NULL) {
    fclose(file);
    return 1;
  }

  int pattern_len = strlen(pattern);

  CheckLastPatternSymbol(pattern, &pattern_len);

  FindPatternByBoyerMoore(file, pattern, pattern_len);

  fclose(file);
  return 0;
}
