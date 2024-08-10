# Strings

String em C são ponteiros. Assim como arrays, elas não existem como conceito
em C.

## String literals

Os string literals são sequencias de caracteres em aspas duplas(`"`).

## Variáveis string

Para assimilar uma string a uma variável, se usa um ponteiro para um `char`.

```c
char *s = "Hello, world!";
```
A variável `s` é um ponteiro para o primeiro caractere na satring, o `H`.

## Strings como arrays

Outra opção para isso é definir a string como um `array`:

```c
char s[14] = "Hello, world!\n";

char s[] = "Hello, world!\n"; // <- O compilador vai resolver isso
```

Isso significa que você pode usar notação de array para acessar caracteres
numa string.

## Inicializadores de string

```c
char *s = "Hello, world!";
char t[] = "Hello, again!";
```

Essas duas inicializações são diferentes. Um string literal, parecido com
um integer literal, tem a memória automaticamente gerenciada pelo compilador.
Com um inteiro, ex. um pedaço de dado com tamanho fixo, o compilador consegue
gerenciar isso facilmente. Mas strings são bestas de tamanho variável que o 
compilador doma jogando um chunk de memória e dando um ponteiro para isso.

Essa forma aponta para o local onde a string foi colocada. Tipicamente, é um
lugar longe do resto da memória do computador, por razões de performance e segurança.

```c
char *s = "Hello, world!";
```

Se você tentar mutar a string com isso:

```c
char *s = "Hello, world!";

s[0] = 'z'; // BAD NEWS: tried to mutate a string literal!
```

Declarar como um array é diferente, o compilador não guarda esses bytes em
outra parte do computador e essa cópia é mutavel:

```c
char []t = "Hello, world!";
t[0] = 'z';

printf("%s\n",t); // "zello, again!"
```

Lembre-se: Se você tem um ponteiro para um string literal, não tente
mutá-lo!

## Pegando a largura da string

Você não pode, o C não rastreia isso. Para isso, existe uma função na lib `<string.h>`
chamada `strlen()` que pode ser usada para computar a largura de qualquer string em bytes.

A função `strlen()` retorna o tipo `size_t`, que é um tipo inteiro então você pode
usar para aritmética de inteiros.

## Terminação de strings

Em C, uma string tem  duas características básicas.

- Um ponteiro para o primeiro caractere da string.
- Um byte com valor zero (ou `NUL` character) em algum lugar na memória após o ponteiro que indica o final da string.

Esse `NUL` pode ser escrito no código como `\0`, apesar de você não precisar
fazer isso na maioria das vezes.

```c
char []t = "Hello, world!"; // Actually "Hello, world!\0" behind the scenes.
```

Com isso em mente, vamos implementar nosso próprio `strlen` que conta o número
de `char`s em uma string até que ela encontre um `NUL`.

<implementado no arquivo 007-strings.c>

## Copiando uma string

Você não pode copiar uma string usando o operador `=`. Tudo o que ela faz é
fazer uma cópia para o ponteiro do primeiro caractere. Para fazer uma cópia
de uma string, você precisa copiar um byte por vez, mas isso é feito mais fácil
usando a função `strcpy()`.
