#include <stdio.h>

struct car {
  char *name;
  float price;
  int speed;
};

void set_price(struct car *c, float new_price);

int main(void) {
  struct car saturn = {.speed = 175, .name = "Saturn SL/2"};

  // Passando o ponteiro do `struct car`  com um preço:
  set_price(&saturn, 799.99);

  printf("Price: %f\n", saturn.price);
};

void set_price(struct car *c, float new_price) { c->price = new_price; }
