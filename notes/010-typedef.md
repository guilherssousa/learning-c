# `typedef`: Making new files

## Teoria

Basicamente você pega um tipo que já existe e cria um `alias` para ele:

```c
typedef int antelope;

antelope x = 10;
```

Você pode pegar qualquer tipo existente e fazer isso. Você pode até
fazer um número de tipos com uma lista separada por vírgula.

```c
typedef int antelope, bagel, mushroom; // todos são int
```

## Escopo

`typedef` segue regras padrão de scoping. Por essa razão, é normal ver
`typedef`s em escopo de arquivo para que todas as funções possam usá-las.

## `typedef` em prática

As vezes uma `struct` será usada com `typedef` para que você não precise
digitar a palavra `struct` várias e várias vezes.

## `typedef` e outros tipos

Eles também são úteis para refatorar. Ex.: seu app tem o tipo `float` em
vários lugares diferentes, mas eles precisam ser mudados para `double`.
Mudar um único `typedef` é mais fácil do que mudar todos esses tipos.

## `typedef` e ponteiro

Você pode fazer um tipo que é um ponteiro. Apesar de não ser tão legal,
porque isso omite que o tipo é um ponteiro.

## Caplitalização

Não importa.

## Arrays e `typedef`

A sintaxe é estranha e é difícil de ver, mas você pode fazer um `typedef`
com um array de alguns itens.
