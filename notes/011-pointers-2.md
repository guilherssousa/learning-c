# Ponteiros 2: Aritmética

## Aritmética de Ponteiros

Você pode fazer matemática com ponteiros, principalmente
adição e subtração.

Ao adicionar 1 a um ponteiro, ele é movido para o próximo item
diretamente seguindo ele na memória.

É **importante** lembrar que conforme movemos ponteiros por aí
e checamos lugares diferentes da memória, precisamos ter certeza
que estamos sempre apontando para um lugar válido na memória antes
que façamos a dereferenciação. Se nós estamos no meio do nada e tentamos
ver o que acontece, o resultado é 'undefined behavior'.

## Adicionando a ponteiros

Vamos ter um array de números.

```c
int a[5] = {11, 22, 33, 44, 55};
```

Vamos pegar um ponteiro para o primeiro item do array:

```c
int a[5] = {11, 22, 33, 44, 55};

int *p = a;
```

Agora, vamos printar o valor fazendo a dereferenciação do ponteiro:

```c
printf("%d\n", *p);
```

Agora vamos usar aritmética de ponteiros para printar o próximo elemento
no array:

```c
printf("%d\n", *(p+1)); // Printa 22
```

O compilador sabe o tamanho do `int` e por isso sabe quantos bytes avançar.

Isso significa que podemos printar um array dessa forma:

```c
int a[5] = {11, 22, 33, 44, 55};

int *p = &a[0];  // Or "int *p = a;" works just as well

for (int i = 0; i < 5; i++) {
    printf("%d\n", *(p + i));  // Same as p[i]!
}
``

## Mudando ponteiros

Vamos mudar o ponteiro em si dessa vez.

```c
int a[] = {11, 22, 33, 44, 55, 999};

int *p = a;

while(*p != 999) {
    printf("%d\n", *p);
    p++; // Honestamente, eu não tenho a menor ideia do que esse código faz.
}
```

## Diminuindo ponteiros

Você pode subtrair ponteiros da mesma forma que adicionamos.

Mas você também pode subtrair dois ponteiros para saber a diferença entre eles.
Por exemplo, calcular quantos `int` tem entre dois `int*`. O problema é que só
funciona com um `array` único. Se os pointers apontarem pra qualquer outra coisa,
rola undefined behavior.

Lembra que strings são `char*`? Com isso podemos fazer uma variante de `strlen()`
para computar a largura da string que usa subtração de ponteiros.


## Equivalência de Arrays e Ponteiros

Agora é hora da fórmula fundamental da equivalência entre arrays e ponteiros

```c 
a[b] == *(a+b)
```

Esses dois são equivalentes e podem ser usados mutualmente!

## Equivalência de Arrays e Ponteiros em chamadas de Função

É aqui onde você vai encontrar esse conceito mais aplicado.

Se você tem uma função que recebe um ponteiro como argumento, por exemplo:

```c 
int my_strlen(char *s);
```

Isso significa que você pode passar tanto um ponteiro ou um array pra essa
função que vai dar na mesma.

```c 
char s[] = "Antelopes";
char *t = "Wombats";

// as duas vão funcionar.
// logo, essas duas chamadas são equivalentes.
int my_strlen(char *s);
int my_strlen(char s[]);
```

## Ponteiros `void`

Você já viu a palavra registrada `void` sendo usada com funções,
mas isso é uma coisa totalmente diferente.

As vezes é útil ter um ponteiro para algo que você não sabe qual é o tipo.

Tem dois casos de uso para isso.

1. Uma função vai operar em algo byte por byte. Por exemplo `memcpy()` copia
bytes da memória de um pontiero para outro, mas esses ponteiros podem apontar
para qualquer tipo. o `memcpy()` tira vantagem do fato que você itera sob
`char*`, entãov ocê está iterando sobre os bytes de um objeto não importa
qual tipo é. 

2. Outra função está chamando uma função que você passou (um _callback_),
e passando dados a você. Você sabe os tipos de dados, mas a função chamando você
não. Então ele passa `void*`, porque não sabe os tipos. E você converte esses
tipos pros que você precisa. O `qsort()` e o `bsearch()` usam essa técnica.

Essa é a assinatura da função `memcpy()`:

```c
void *memcpy(void *s1, void *s2, size_t n);
```

Essa função copia `n` bytes de memória começando pelo endereço `s2` para o
endereço começando no endereço `s1`.

Veja que `s1` e `s2` são `void*`. 

Se não tivéssemos o `void*`, teriamos que escrever funções especializadas do
`memcpy()` para cada tipo:

```c
memcpy_int(int *a, int *b, int count);
```

O `void*` é usado para evitar isso. O poder do `void*` é trabalhar com funções
onde você não se importa com o tipo e tá tudo bem.

Lembre-se: Com grandes poderes vem grandes responsabilidades. Existem alguns
limites para essa prática:

1. Você não pode fazer aritmética num `void*`.
2. Você não pode dereferenciar um `void*`.
3. Você não pode usar o `->` num `void*`.
4. Você não pode usar a notação de array num `void*`.

O truque é converter o `void*` para outro tipo antes de usá-lo:

```c
char a = 'X';

void *p = &a;
char *q = p;

printf("%c\n", *p); // <- Erro: tentou dereferenciar um `void*`
printf("%c\n", *q); // <- Printa "X" normalmente.
```

Implementamos também a nossa versão do `memcpy()` no `011-pointers-2.c`.


