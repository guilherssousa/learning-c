# Pointers

## Memória e variáveis

Para fazer memória ficar fácil de lidar, cada byte de memória é identificado 
com um `int`. Esses ints são sequencialmente incrementados de acordo com que
você avança na memória.

Você pode pensar como um monte de caixas numeradas, onde cada caixa guarda um byte
de dados. O número que representa cada caixa é chamado de *endereço*.

Nem todos os tipos de dados usam apenas um byte. Por exemplo, um `int` costuma ser
4 bytes, assim como um `float`, mas depende do sistema. Você pode usars o `sizeof`
para determinar quantos bytes de memoria um tipo usa.

> Informação curiosa: Quando você usa um tipo de dado que usa mais de um byte de
> memória, os bytes são sempre guardados de forma adjacente um do outro em memória.

Um ponteiro é uma variável que guarda um endereço.

Você pode ter uma variável que guarda um valor. E esse vlaor está em memória em
algum endereço. Você pode ter uma variável ponteiro que guarda o endereço dessa
variável de dados.

O operador `address-of` (que é um &) é usado para achar o endereço do dado.

Os ponteiros tem um especificador de formato especial para o `printf()`: `%p`.

```c
#include <stdio.h>

int main(void)
{
    int i = 10;

    printf("The value of i is %d\n", i);
    printf("And its address is %p\n", (void *)&i);
}
```

O código acima contem um **cast** onde fazemos a coerção da expressão `&i`
para o tipo `void*`. Isso é para impedir o compilador de soltar um aviso.
Apenas ignore enquanto não cobrimos isso.

É um ponteiro porque nos deixa saber onde está `i` na memória. Assim como
um endereço numa folha de papel nos deixa saber como achar uma casa em particular.

## Tipos de ponteiros

Você pode guardar um pointeiro em uma variável para ser usada depois.
Os tipos de ponteiros podem ser identificados por causa do asterísco antes do nome
da variável e depois do seu tipo:

```c
int main(void)
{
    int i;
    int *p;
}
```

Aqui nós temos uma variável ponteiro, que pode apontar para outros `int`s.
Isto é, pode guardar o endereço de outros `int`s. Nós sabemos que aponta para
`int`s, porque é do tipo `int*`

Quando você faz um assignment para uma variável ponteiro, o tipo do lado direito 
precisa ser do mesmo tipo do ponteiro da variável. Felizmente, qunado você tira
o `address-of` de uma variável, o tipo resultante é um ponteiro para aquele tipo
de variável, então assigments assim são perfeitos:

```c
int i;
int *p;

p = &i;
```

## Dereferencia

Quando você tem um ponteiro para uma variável, você pode usar a variável original
pelo ponteiro dereferenciando o valor.

Se você tem uma variável chamada `i` e um ponteiro para `i` chamado `p`, você
pode usar o ponteiro dereferenciado `p` exatamente como se fosse a variável original
`i`!

O operador de dereferencia é chamado de indirection operator, porque você está
acessando valores indiretamente pelo ponteiro, e é o *, denovo.

## Passando ponteiros como argumentos

O poder real de ponteiros vem quando você começa a passar eles para funções.
E se você pudesse trazer mais de um pedaço de dados da função como retorno?

O que acontece quando você passa um ponteiro como um argumento de função?
A cópia do ponteiro é colocada no seu respectivo parâmetro! Assim, a função
pode fazer a dereferencia da cópia do ponteiro para receber a variável original!

A função não consegue ver a variável em si, mas consegue deferenciar um ponteiro
para aquela variável.

---

A função `increment()` recebe uma copia do ponteiro. Tanto o ponteiro original `j`
(na função `main()`) quanto a cópia do ponteiro `p` (o parâmetro em `increment()`)
apontam para o mesmo endereço, o que guarda o valor `i`.

Dereferenciar qualquer um dos dois permitrá modificar a variável original `i`!
A função pode modificar a variável em outro escopo!

O exemplo abaixo é mais comumente escrito apenas passando o ponteiro na lista
de argumentos:

```c
printf("i is %d\n", i);
increment(&i);
printf("i is %d\n", i);
```

## O ponteiro NULL

Qualquer ponteiro de qualquer tipo pode ser definida para um valor especial
chamado NULL. Isso indica que o ponteiro aponta para lugar nenhum

Sabendo que ela não aponta para lugar nenhum, dereferenciar é um comportamento
indefinido, e provavelmente irá resultar num crash:

```cs
int *p = NULL;

*p = 12;
```

## Uma nota sobre declaração de ponteiros

A sintaxe para declarar ponteiros pode ficar um pouco estranha.

```c
int a;
int b;
```

Essa sintaxe pode ser condensada em uma única linha:

```c
int a, b;
```

Onde `a` e `b` são inteiros. Sem problema.

Mas e isso?

```c
int a;
int *p;
```

Podemos fazer isso em uma linha? Podedmos, mas onde o * vai?

A regra é que o * vai na frente de qualquer variável que é um ponteiro.
Isto é, o `*` não é parte do `int` nesse exemplo. É parte da variável `p`.

```c
int a, *p;
```

É importante notar que o próximo exemplo não declara dois ponteiros:

```c
int *p, q; // p é um ponteiro para um int; q é apenas um int
```

Isso pode ser particularmente confuso se o programador escrever esse
código válido que é funcionalmente igual ao anterior:

```c
int* p, q; // p é um ponteiro para um int; q é apenas um int
```

Então, olhe esse exemplo e determine quais variáveis são ponteiros
e quais não são:

```c
int *a, b, c, *d, e, *f, g, h, *i; // a, d, f e i são ponteiros.
```

## `sizeof` e ponteiros

Se você usar um `sizeof` num ponteiro, ele irá trazer o tamanho do tipo do ponteiro.
