#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int i = 2;
  float f = 3.14;

  char *s = "Hello, world!";

  printf("%s i = %d and f = %f!\n", s, i, f);

  bool x = true;

  if (x) {
    printf("x is true!\n");
  }

  printf("The number %d is %s.\n", x, x % 2 == 0 ? "even" : "odd");

  int r;

  do {
    r = rand() % 100;
    printf("%d\n", r);
  } while (r != 37);
}
