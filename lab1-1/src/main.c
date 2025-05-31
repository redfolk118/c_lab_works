#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define HASH_BASE 3

void ReadPattern(FILE *file, unsigned char **pattern, int *pattern_len) {
  char buffer[BUFFER_SIZE];
  if (fgets(buffer, BUFFER_SIZE, file) == NULL) {
    *pattern = NULL;
    *pattern_len = 0;
    return;
  }
  *pattern_len = strlen(buffer);
  if (buffer[*pattern_len - 1] == '\n') {
    buffer[--(*pattern_len)] = '\0';
  }
  *pattern = malloc((*pattern_len + 1) * sizeof(unsigned char));
  memcpy(*pattern, buffer, *pattern_len + 1);
}

bool ReadNextChunk(FILE *file, unsigned char **buffer, int *buffer_len,
                   int overlap_len) {
  if (*buffer == NULL) {
    *buffer = malloc(BUFFER_SIZE * sizeof(unsigned char));
    *buffer_len = BUFFER_SIZE;
  }

  if (overlap_len > 0) {
    memmove(*buffer, *buffer + (*buffer_len - overlap_len), overlap_len);
  }

  const int bytes_read = fread(*buffer + overlap_len, sizeof(unsigned char),
                               *buffer_len - overlap_len, file);

  if (bytes_read <= 0) {
    *buffer_len = overlap_len;
    return false;
  }

  *buffer_len = bytes_read + overlap_len;
  return true;
}

long long CalculateHash(const unsigned char *str, int len) {
  long long hash = 0;
  long long power = 1;
  for (int i = 0; i < len; i++) {
    hash += (str[i] % HASH_BASE) * power;
    power *= HASH_BASE;
  }
  return hash;
}

void RecountWindowHash(long long *window_hash, unsigned char *buffer,
                       int symbol_idx, int pattern_len, int power) {
  *window_hash = (*window_hash - (buffer[symbol_idx] % HASH_BASE)) / HASH_BASE +
                 (buffer[symbol_idx + pattern_len] % HASH_BASE) * power;
}

void CompareAndPrintMatches(unsigned char *buffer, int buffer_len,
                            const unsigned char *pattern, int pattern_len,
                            int start_idx, long long *window_hash,
                            int pattern_hash, int power) {
  for (int i = 0; i <= buffer_len - pattern_len; i++) {
    if (*window_hash == pattern_hash) {
      for (int j = 0; j < pattern_len; j++) {
        printf("%d ", i + j - pattern_len + 1 + start_idx + 1);
        if (buffer[i + j] != pattern[j]) {
          break;
        }
      }
    }

    if (i < buffer_len - pattern_len) {
      RecountWindowHash(window_hash, buffer, i, pattern_len, power);
    }
  }
}

long long CalculateHashPower(int exp) {
  int power = 1;
  for (int i = 0; i < exp - 1; i++) {
    power *= 3;
  }
  return power;
}

void DoRabinKarp(const unsigned char *pattern, int pattern_len, FILE *file) {
  unsigned char *buffer = NULL;
  int buffer_len = 0;
  int overlap_len = pattern_len - 1;
  int start_idx = 0;

  long long pattern_hash = CalculateHash(pattern, pattern_len);
  printf("%lld ", pattern_hash);

  long long max_base_power = CalculateHashPower(pattern_len);

  while (true) {
    const bool has_data =
        ReadNextChunk(file, &buffer, &buffer_len, overlap_len);

    long long window_hash = CalculateHash(buffer, pattern_len);

    CompareAndPrintMatches(buffer, buffer_len, pattern, pattern_len, start_idx,
                           &window_hash, pattern_hash, max_base_power);

    start_idx += buffer_len - pattern_len + 1;

    if (!has_data) {
      break;
    }
  }
  free(buffer);
}

int main() {
  FILE *file = fopen("in.txt", "r");

  unsigned char *pattern = NULL;
  int pattern_len = 0;
  ReadPattern(file, &pattern, &pattern_len);

  if (pattern == NULL || pattern_len == 0) {
    fclose(file);
    return 1;
  }

  DoRabinKarp(pattern, pattern_len, file);

  free(pattern);
  fclose(file);

  return 0;
}
