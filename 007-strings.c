#include <stdio.h>
#include <string.h>

size_t my_strlen(char *s) {
  size_t count = 0;

  while (s[count] != '\0') {
    count++;
  }

  return count;
}

int main(void) {
  char *s = "Hello, world!";
  char t[100];

  strcpy(t, s);
  t[0] = '0';

  printf("The string is %zu bytes long.\n", my_strlen(s));
  printf("s is %s\n", s);
  printf("t is %s\n", t);
}
