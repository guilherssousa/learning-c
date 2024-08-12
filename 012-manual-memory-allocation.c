#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reallocating() {
  float *p = malloc(sizeof *p * 20);

  for (int i = 0; i < 20; i++) {
    p[i] = i / 20.0;
  }

  // But wait! let's actually make this an array of 40 elements
  float *new_p = realloc(p, sizeof *p * 40);

  if (new_p == NULL) {
    printf("Error reallocating\n");
    return 1;
  }

  p = new_p;

  for (int i = 20; i < 40; i++) {
    p[i] = 1.0 + (i - 20) / 20.0;
  }

  for (int i = 0; i < 40; i++) {
    printf("%f\n", p[i]);
  }

  free(p);
  return 0;
}

char *readline(FILE *fp) {
  int offset = 0;
  int bufsize = 4;
  char *buf;
  int c;

  buf = malloc(bufsize);

  if (buf == NULL) {
    return NULL;
  }

  // Loop principal. Ler até o `newline` ou `EOF`.
  while (c = fgetc(fp), c != '\n' && c != EOF) {
    // Cheque se há espaço para o buffer ocntando com o byte extra
    // para o NUL terminator.
    if (offset == bufsize - 1) {
      bufsize *= 2;

      char *new_buf = realloc(buf, bufsize);

      if (new_buf == NULL) {
        free(buf); // não é possível continuar a execução sem o espaço
        return NULL;
      }

      buf = new_buf;
    }

    buf[offset++] = c;
  }

  // assumindo que chegamos numa newline ou num EOF

  // se chegamos no EOF e não limos nada, returne NULL
  if (c == EOF && offset == 0) {
    free(buf);
    return NULL;
  }

  // Diminuir o buffer
  if (offset < bufsize - 1) {
    char *new_buf = realloc(buf, offset + 1); // +1 for NUL terminator

    if (new_buf != NULL) {
      buf = new_buf;
    }
  }

  buf[offset] = '\0';

  return buf;
}

void alligned_realloc(void) {
  char *p = aligned_alloc(64, 256);

  strcpy(p, "Hello, World!");
  printf("%s\n", p);

  free(p);
}

int main(void) {
  reallocating();
  alligned_realloc();

  FILE *fp = fopen("foo.txt", "r");

  char *line;

  while ((line = readline(fp)) != NULL) {
    printf("%s\n", line);
    free(line);
  }

  fclose(fp);
}
