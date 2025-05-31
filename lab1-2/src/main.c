#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERN_LENGTH 20

void FixLastPatternSymbol(unsigned char *pattern, int *pattern_len) {
  if (pattern[*pattern_len - 1] == '\n' && *pattern_len > 0) {
    pattern[*pattern_len - 1] = 0;
    (*pattern_len)--;
  }
}

void CreatePrefixArray(const unsigned char *pattern, int pattern_length,
                       int *prefix_suffix) {
  int match_length = 0;
  prefix_suffix[0] = 0;
  int index = 1;

  while (index < pattern_length) {
    if (pattern[index] == pattern[match_length]) {
      match_length++;
      prefix_suffix[index] = match_length;
      index++;
    } else {
      if (match_length != 0) {
        match_length = prefix_suffix[match_length - 1];
      } else {
        prefix_suffix[index] = 0;
        index++;
      }
    }
  }
}

int ReadNextSymbol(FILE *file, unsigned char *text, int *compare_index) {
  int input_symbol = fgetc(file);
  if (input_symbol != EOF) {
    text[*compare_index] = input_symbol;
  }
  *compare_index = (*compare_index + 1) % (MAX_PATTERN_LENGTH * 2);
  return input_symbol;
}

void SearchSubstring(FILE *file, const unsigned char *pattern,
                     int pattern_length) {
  unsigned char *text =
      (unsigned char *)malloc(MAX_PATTERN_LENGTH * 2 * sizeof(unsigned char));
  int text_length =
      fread(text, sizeof(unsigned char), MAX_PATTERN_LENGTH * 2, file);

  int *prefix_suffix = malloc(pattern_length * sizeof(int));
  CreatePrefixArray(pattern, pattern_length, prefix_suffix);

  for (int i = 0; i < pattern_length; i++) {
    printf("%d ", prefix_suffix[i]);
  }

  int text_index = 0;
  int pattern_index = 0;

  if (pattern_length == 0) {
    free(text);
    free(prefix_suffix);
    return;
  }

  int compare_index = 0;
  while (text_index <= text_length - (pattern_length - pattern_index)) {
    if (pattern[pattern_index] == text[compare_index]) {
      text_length += (ReadNextSymbol(file, text, &compare_index) != EOF);
      text_index++;
      pattern_index++;

      if (pattern_index == pattern_length) {
        printf("%d %d ", text_index - pattern_length + 1, pattern_length);
        pattern_index = prefix_suffix[pattern_index - 1];
      }
    } else {
      if (pattern_index > 0) {
        printf("%d %d ", text_index - pattern_index + 1, pattern_index);
        pattern_index = prefix_suffix[pattern_index - 1];
      } else {
        text_length += (ReadNextSymbol(file, text, &compare_index) != EOF);
        text_index++;
      }
    }
  }
  free(text);
  free(prefix_suffix);
}

int main() {
  FILE *file = fopen("in.txt", "r");

  unsigned char pattern[MAX_PATTERN_LENGTH];
  if (fgets((char *)pattern, MAX_PATTERN_LENGTH, file) == NULL) {
    fclose(file);
    return 0;
  }

  int pattern_len = strlen((const char *)pattern);
  if (pattern_len == 1) {
    fclose(file);
    return 0;
  }

  FixLastPatternSymbol(pattern, &pattern_len);

  SearchSubstring(file, pattern, pattern_len);

  fclose(file);

  return 0;
}
