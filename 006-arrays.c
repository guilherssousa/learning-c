#include <stdio.h>

void times2(int *a, int len);
void times3(int a[], int len);
void times4(int a[5], int len);

void double_array(int *a, int len) {
  for (int i = 0; i < len; i++)
    a[i] *= 2;
}
// void print_2d_array(int a[][3]) <- As duas chamadas abaixo são equivalentes
void print_2d_array(int a[2][3]) {
  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 3; col++) {
      printf("%d ", a[row][col]);
    }
    printf("\n");
  }
}

int main(void) {
  int i;
  float f[4];

  f[0] = 3.14159;
  f[1] = 1.41421;
  f[2] = 1.61803;
  f[3] = 2.71828;

  for (i = 0; i < 4; i++) {
    printf("%f\n", f[i]);
  }

  int x[5] = {11, 22, 33, 44, 55};

  printf("%zu\n", sizeof x);
  printf("%zu\n", sizeof(int));
  printf("%zu\n", sizeof x / sizeof(int));

  times2(x, 5);
  times3(x, 5);
  times4(x, 5);

  double_array(x, 5);

  for (i = 0; i < 5; i++) {
    printf("%d\n", x[i]);
  }

  int _2d[2][3] = {{1, 2, 3}, {4, 5, 6}};

  print_2d_array(_2d);
}

// Passando um ponteiro para o primeiro elemento
void times2(int *a, int len) {
  for (int i = 0; i < len; i++) {
    printf("%d\n", a[i] * 2);
  }
};

// A mesma coisa, mas usando a notação de array
void times3(int a[], int len) {
  for (int i = 0; i < len; i++) {
    printf("%d\n", a[i] * 3);
  }
};

// A mesma coisa, mas usando a notação de array com tamanho
void times4(int a[5], int len) {
  for (int i = 0; i < len; i++) {
    printf("%d\n", a[i] * 3);
  }
};
