#include "prefix_tree.h"
#include <stdio.h>
#include <string.h>

int main(void) {
  FILE *in = fopen("in.txt", "r");
  FILE *out = fopen("out.txt", "w");
  int N;
  if (fscanf(in, "%d\n", &N) != 1) {
    fclose(in);
    fclose(out);
    return 0;
  }
  Node *root = CreateNode(NULL, false);
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < N; i++) {
    if (!fgets(buffer, sizeof(buffer), in)) {
      fclose(in);
      fclose(out);
      return 0;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    InsertWord(root, buffer);
  }
  if (!fgets(buffer, sizeof(buffer), in)) {
    fprintf(out, "None\n0");
  } else {
    PrintWordsWithPrefix(root, buffer, out);
    fprintf(out, "\n%d", CountNodes(root));
  }
  fclose(in);
  fclose(out);
  FreeTree(root);
  return 0;
}
