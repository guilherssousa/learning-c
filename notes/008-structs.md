# Structs

`struct` é um tipo definido pelo usuário que guarda múltiplos tipos de data,
potencialmente de tipos diferentes.

Classes e objetos não existem em C de forma nativa, você pode pensar no `struct`
como uma classe com apenas membros de data e nenhum método.

## Declarando uma `struct`

```c
struct car {
  char *name;
  float price;
  int speed;
};
```

O nome inteiro do tipo é `struct car`.

```c
struct car saturn;  // Variable "saturn" of type "struct car"

// você pode assimillar os valores de duas formas:
saturn.name = "Saturn SL/2";
saturn.price = 15999.99;
saturn.speed = 175;

// Now with an initializer! Same field order as in the struct declaration:
struct car saturn = {"Saturn SL/2", 16000.99, 175};

// ou pode ser específico com os inicializadores
struct car saturn = {.speed=175, .name="Saturn SL/2"};
```

## Passando structs para funções

Você pode fazer algumas coisas para passar um `struct` para uma função:

1. Passar o `struct`
2. Passar um ponteiro para o `struct`

Existem dois casos onde faz sentido passar um ponto para o struct:

- Você precisa modificar o `struct` original.
- O struct é grande demais e copiar seria uma operação custosa.

É muito mais comum passar o ponteiro para o `struct`.

A declaração de uma função que recebe um ponteiro de um struct parece com isso:

```c
struct car {
  char *name;
  float price;
  int speed;
};

void set_price(struct car *c, float new_price);
```

Se você tentar fazer a função assim:
```c
void set_price(struct car *c, float new_price) {
    c.price = new_price;  // error!!
}
```

A função irá retornar um erro. O operador `.` só funciona em `structs`,
e não em ponteiros para `struct`s. Para resolver isso, basta dereferenciar
o ponteiro que resulta no `struct original`.

```c
void set_price(struct car *c, float new_price) {
    (*c).price = new_price;  // <- Isso funciona, apesar de ser feio.

    c->price = new_price; // <- Isso é um syntax-sugar do C que equivale a mesma coisa.
}
```

## Copiando e retornando `struct`s

Para copiar é muito fácil:


```c
struct car a, b;

b = a;
```

Retornar uma `struct` de uma função faz uma cópia similar ao valor recebido.
Importante saber que isso não é um `deep-copy`, todos os valores são copiados
da forma que são, incluindo ponteiros para coisas.

## Comparando `struct`s

Só há uma forma segura de comparar: Comparar cada campo por vez.

Outros métodos: [8.6 Comparing structs](https://beej.us/guide/bgc/html/split/structs.html)
