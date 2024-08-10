# Variables and Statements

Tipos de variaveis em C:

- Integer `int`
- Floating point `float`
- Caractere unico `char`
- String `char *`

O C faz um esforco para converter automaticamente a maioria dos tipos numericos.

## Tipos Booleanos

Historicamente, o C nao tem tipos booleanos, e muitos podem dizer que ainda nao tem.

Em C, 0 significa falso e nao-zero significa verdadeiro.

Entao 1 e verdadeiro, assim como -37. E 0 e falso.

Voce pode declarar booleanos como inteiros.

Se voce incluir o headerfile `stdbool.h`, voce pode ter acesso a nomes mais simbolicos
que farao as coisas parecerem mais familiares, especialmente um tipo `bool` e os valores
`true` e `false`.

## Operadores e Expressoes

Essas sao as operacoes disponvieis em C:

```c 
i = i + 3; // Adicao
i = i - 8; // Subtracao
i = i * 9; // Multiplicacao
i = i / 2; // Divisao
i = i % 5; // Modulo
```

Todos eles tem uma sintaxe mais portatil:


```c 
i += 3; // O mesmo que i = i + 3;
i -= 8; // O mesmo que i = i - 8;
i *= 9; // O mesmo que i = i * 9; 
i /= 2; // O mesmo que i = i / 2;
i %= 5; // O mesmo que i = i % 5; 
```

Nao ha funcao de exponenciacao. Voce tera que usar uma das funcoes `pow()` do `math.h`

Vamos ver algumas das coisas estranhas do C

### Operador ternario

O C tambem inclui um operador ternario.

```c
y += x > 10 ? 10 : 37;
```

### Pre e pos incremento e decremento

Esses sao operadores lendarios:

```c
i++;
i--;
```

Voce tambem a versao pre decremento e pre incremento

```c
++i;
--i;
```

### O operador virgula

Essa e uma forma incomum de separar expressoes que vao rodar da esquerda pra direita

```c
x = 10, y = 20;
```

### O operador sizeof

Esse e o operador quet e conta o tamanho em bytes de uma variavel ou um tipo
particular em memoria.

O C usa um tipo especial para representar o retorno do sizeof, chamado `size_t`.
E um integer sem sinal que pode carregar o valor de tudo que voce pode dar ao sizeof.

`size_t` aparece bastante em varios lugares diferentes onde contagens de coisas sao
passadas ou retornadas. Pensa nisso como um valor que representa uma contagem

Lembre-se: E o tamanho em bytes do tipo da expressao, nao do tamanho da expressoa em si.

## Fluxo de controle

Voce pode escrever um `if` de duas formas:

```c
if (x === 10) printf("x is 10\n");

// ou...

if (x === 10) {
    printf("x is 10\n");
}
```

O else funciona da mesma forma:

```c
if (x === 10) {
    printf("x is 10\n");
} else {
    printf("x is not 10\n");
}
```

## O statement `while`

Serve para te dar um loop basico 

## O statement `do-while`

Ele e basicamente a mesma coisa que o `while`, a diferenca e que quando a condicional
e falsa, o do-while ira executar uma unica vez, enquanto o while nao executara nenhuma.

## Switch

A unica informacao util que deveriamos saber em C e que e necessario dar break no Switch.

Tambem, o `switch` só funciona bem com integer types. Não use floats nem strings aqui. Apesar que voce pode usar chars e enums.

## Listas de parametros vazias

Voce pode ver esses em códigos mais antigos, mas você não deveria fazer um desses
no seu código novo. Sempre use `void` para indicar que uma função não recebe
parâmetros.
