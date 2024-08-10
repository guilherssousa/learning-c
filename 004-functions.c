#include <stdio.h>

int foo(void);

int plus_one(int n) { return n + 1; }

void hello(void) { printf("Hello, world!\n"); }

void increment(int a) { a++; }

int main(void) {
  int i = 10, j, f;

  j = plus_one(i);

  printf("i + l is %d\n", j);

  hello();

  increment(i);

  printf("i is %d\n", i);

  f = foo();

  printf("%d\n", i);
}

int foo(void) { return 3490; }
