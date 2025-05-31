#include "string_units.h"
#include <stdlib.h>
#include <string.h>

char *DuplicateString(const char *input_string) {
  size_t input_length = strlen(input_string);
  char *result = malloc(input_length + 1);
  memcpy(result, input_string, input_length + 1);
  return result;
}

char *CopyString(const char *input_text, int copy_length) {
  char *text_copy = malloc((size_t)copy_length + 1);
  memcpy(text_copy, input_text, copy_length);
  text_copy[copy_length] = '\0';
  return text_copy;
}
