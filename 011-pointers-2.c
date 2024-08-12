#include <stdio.h>
#include <string.h>

int my_strlen(char *s) {
  // começa escaneando do começo da string
  char *p = s;

  // avança até o caractere NUL
  while (*p != '\0') {
    p++;
  }

  // retorna a diferença dos ponteiros
  return p - s;
}

int *my_memcpy(void *dest, void *src, int len) {
  char *s = src, *d = dest; // <- Os `void*` são convertidos para `char*` porque
                            // são 1 bytes e podem fazer aritmética

  while (len--) {
    *d++ = *s++;
  }

  return dest;
}

int main(void) {
  char *text = "Hello, world!";
  int my_len = my_strlen(text);
  int gnu_len = strlen(text);

  if (my_len == gnu_len) {
    printf("Estão certo! Ambas tem %d\n", my_len);
  }

  int a[] = {11, 22, 33, 44, 55};

  for (int i = 0; i < 5; i++) {
    printf("%d\n", *(a + i));
  }
}
