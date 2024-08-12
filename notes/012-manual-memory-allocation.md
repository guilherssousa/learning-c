# Alocação Manual de Memória

Em C, algumas variáveis são automaticamente alocadas e desalocadas quando elas entram e saem de escopo. O gerenciamento manual de memória entra em ação quando precisamos que um pedaço persista mais do que em apenas um bloco.

Você pode falar explicitamente para o C para alocar para você um certo número de bytes que você pode usar como preferir. E esses bytes ficarão alocados até que você explicitamente libere essa memória.

É importante liberar a memória que você usou! Se não, chamamos isso de _Memory Leak_ e seu processo irá continuar a reservar aquele pedaço de memória até que o processo seja encerrado.

- Se você alocou manualmente, você precisa liberá-lo manualmente quando terminar de usar.

Em C, devs dizem que as variáveis automáticas e locais são alocadas "na _stack_" e as memórias alocadas manualmente estão "no _heap_". A especificação não fala sobre nenhuma dessas coisas, mas todos os desenvolvedores de C saberão sobre o que você está falando.

Todas as funções usadas neste capítulo podem ser encontradas no `<stdlib.h>`.

## Alocando e Desalocando, `malloc()` e `free()`

A função `malloc()` aceita um número de bytes para alocar, e retorna um ponteiro `void*` para o bloco da memória recém-alocada.

Já que é um `void*`, você pode alocar ele para qualquer tipo que você quiser, isso vai corresponder de alguma forma com o número de bytes que você está alocando.

Se precisamos alocar espaço o suficiente para um único `int`, podemos passar `sizeof(int)` para o `malloc()`. Depois que terminamos de usar a nossa memória alocada, podemos chamar `free()` para indicar que terminamos de usar a memória e ela pode ser usada para algo diferente. Como um argumento, passamos o mesmo ponteiro que recebemos de `malloc()` (ou uma cópia dele). Usar um espaço de memória depois de ser liberado é uma _undefined behavior_.

```c
int *p = malloc(sizeof(int));

*p = 12;

printf("%d\n", *p);

free(p);

*p = 3490; // <- undefined behavior
```

No exemplo acima, não existe nenhum benefício para isso, poderíamos ter simplesmente usado uma variável `int p` e daria certo. Mas iremos ver como a habilidade de alocar memória tem suas vantagens, especialmente com estruturas de dados mais complexas.

Uma coisa que você vai ver bastante toma vantagem do fato que o `sizeof` pode te dar o tamanho do tipo resultante de qualquer expressão constante. Então você pode colocar uma variável lá também, e usar isso. Aqui segue um exemplo parecido com o anterior:

```c
int *p = malloc(sizeof *p);
```

## Checando erros

Todas as funções de alocação retornam um ponteiro para o novo pedaço de memória, ou `NULL` se a memória não puder ser alocada por alguma razão.

Alguns sistemas operacionais podem ser configurados para que o `malloc()` nunca retorne `NULL`, até quando você esteja fora de memória. Mas, independente disso, você sempre deve programar pensando em proteções.

```c
int *x;

x = malloc(sizeof(int) * 10);

if (x == NULL) {
	printf("Error allocating 10 ints\n");
	// do something here to handle it
}
```

Aqui está um padrão comum que você irá ver, que é fazer o _assignment_ e a condição na mesma linha:

```c
int *x;

if ((x = malloc(sizeof(int) * 10)) == NULL)
	printf("Error allocating 10 ints\n");
```

## Alocando espaço para um _Array_

Em C, um _Array_ é um monte da mesma coisa alocado sequencialmente numa faixa de memória.

Nós podemos alocar um pedaço de memória. Se quiséssemos alocar 3490 bytes de memória, poderíamos simplesmente pedir:

```c
char *p = malloc(3490);
```

E, claro! Esse é um _array_ de 3490 `char`s (ou, uma _string_), sabendo que cada `char` é 1 byte. Ou seja, `sizeof(char)` é 1.

> Nota: Não tem nenhuma inicialização feita com a memória recém-alocada. É cheia de lixo. Limpe com `memset()` se quiser, ou veja o `calloc()` abaixo.

Nós podemos simplesmente multiplicar o tamanho da coisa que queremos pelo número de elementos que desejamos e acessá-los usando ou o ponteiro ou a notação de _array_.

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int *p = malloc(sizeof(int)*10);

	for(int i = 0; i < 10; i++) {
		p[i] = i * 5;
	}

	for(int i = 0; i < 10; i++) {
		printf("%d\n", p[i]);
	}

	free(p);
}
```

A chave está na linha do `malloc()`. Se nós sabemos que cada `int` toma `sizeof(int)` bytes para segurar, e nós queremos 10 deles, podemos simplesmente alocar exatamente aquela quantidade com: `sizeof(int) * 10`. Esse truque funciona para todo tipo. Apenas passe para o `sizeof` e multiplique pelo número do array.

## Uma alternativa: `calloc()`

Existe outra função de alocação que funciona similarmente ao `malloc()`, com duas diferenças chave:

- Ao invés de um argumento único, você passa o tamanho de um elemento e o número de elementos que você deseja alocar.
- Limpa a memória alocada para zero.

Você ainda pode usar o `free()`  para desalocar a memória recebida pelo `calloc()`.

Aqui está uma comparação entre o `calloc()` e o `malloc()`.

```c 
// Alocar espaço para dez inteiros com o calloc(), inicializados a 0:
int *p = calloc(10, sizeof(int));

// Alocar espaço para dez inteiros com o malloc(), inicializados a 0:
int *q = malloc(10 * sizeof(int));
memset(q, 0, 10 * sizeof(int));
```

## Mudando tamanho alocado com `realloc()`

Se você já alocou 10 `int`s, mas decidiu que precisa de 20, o que fazer?

Uma opção é alocar um novo espaço e `memcpy()` a memória. Mas as vezes você não precisa mover nada. E tem uma função que é inteligente o suficiente para fazer a coisa certa em todas as circunstâncias: `realloc()`.

Ela recebe um ponteiro para uma memória já alocada (pelo `malloc()` ou pelo `calloc()`) e o tamanho para a nova região de memória.

Então ela cresce ou diminui aquela memória e retorna um ponteiro para isso. As vezes ela pode retornar o mesmo ponteiro (se os dados não precisam ser copiados para outro lugar), ou pode retornar um ponteiro novo (se os dados precisaram ser copiados).

Tenha certeza que quando você chama o `realloc()`, você especifica o número de bytes para alocar!

```c 
num_floats *= 2;

np = realloc(p, num_floats); // <- Errado

np = realloc(p, num_floats * sizeof(float)); // <- Melhor
```

Vamos alocar  um array de 20 `float`s, e depois mudar de ideia e fazer um array de 40.

Vamos assimilar o retorno de `realloc()` num novo ponteiro para ter certeza de que ele não é `NULL`. Se não, podemos realocar para um novo ponteiro. Se apenas assimilarmos o ponteiro para o original, perderíamos o ponteiro se a função retornasse `NULL`.

<implementado no 012-manual-memory-allocation.c>

## Lendo linhas de largura arbitrária

Agora, é hora de um exemplo completo:
1. Uso do `realloc()` para crescer um _buffer_ conforme lemos mais dados.
2. Uso do `realloc()` para diminuir o tamanho do _buffer_ para o tamanho perfeito depois que completamos a leitura.

O que vemos aqui é um loop que chama o `fgetc()` várias e várias vezes para adicionar ao final do _buffer_ até que vejamos que o último caractere seja um `\n`.

Assim que ele acha o `newline`, ele diminui o buffer para o tamanho perfeito.

<implementado no 012-manual-memory-allocation.c>

Quando estamos aumentando o tamanho da memória assim, é comum (entretanto, dificilmente uma regra) dobrar o espaço necessário para cada etapa apenas para minimizar o número de `reallocs()` que irão acontecer.

Você pode notar que o `readline()` retorna um ponteiro para o buffer alocado com o `malloc()`. Assim, o usuário fica responsável por dar `free()` na memória quando terminar de usar.
## `realloc()` com `NULL`.

Hora da curiosidade! As duas linhas abaixo são equivalentes.

```c
char *p = malloc(3490);
char *p = realloc(NULL, 3490);
```

Isso é conveniente quando você tem algum tipo de loop de alocação e você não quer deixar o primeiro `malloc()`  de forma especial:

```c
int *p = NULL;
int length = 0;

while (!done) {
	length += 10;
	p = realloc(p, sizeof * p * length);
}
```

Nesse exemplo, não precisamos de um `malloc()` inicial porque o `p` já era `NULL`.

## Alocações alinhadas

> Você provavelmente não vai precisar disso.

Existe uma coisa chamada _alinhamento de memória_, que quer dizer sobre o endereço de memória sendo múltiplo de um certo número.

Por exemplo, um sistema pode pedir que os valores de 16-bit comecem em endereços de memória que sejam múltiplos de 2. Ou que dados de 64-bits comecem em endereços que são múltiplos de 2, 4 ou 8, por exemplo. Depende da CPU.

Alguns sistemas necessitam desse tipo de alinhamento para acessar a memória mais rapidamente, alguns até mesmo para acessar a memória de qualquer forma.

Se você usa o `malloc()`, `calloc()` ou o `realloc()`, o C vai te dar um chunk de memória que é bem alocado para qualquer valor, incluindo `struct`. Funciona em qualquer caso.

Mas as vezes você saberá que os dados precisam ser alinhados em um limite menor, ou até ser alinhados em um maior, por alguma razão. Isso é comum com programação de sistemas embarcados.

Você pode especificar o alinhamento com o `aligned_alloc()`.

O alinhamento é um inteiro potencia de dois maior que zero, então `2`, `4`, `8`, `16`, etc. E você pode passar isso para o `aligned_alloc()` antes dos números de bytes que você está interessado.

Outra restrição é que o número de bytes que você aloca precisa ser múltiplo do alinhamento. Mas isso pode mudar.

> O `realloc()` não te dá garantias de alinhamento. Então se você precisa pegar um espaço de memória alinhado, você precisa fazer do jeito difícil com o `memcpy()`.

Aqui está uma função não padrão do `aligned_realloc()`, em caso você precise:

```c
// Essa implementação sempre irá copiar os dados, enquanto o `realloc` evita fazer isso se puder. Então isso não é muito eficiente. Evite de realocar dados que estejam alocados de forma alinhada.
void *aligned_realloc(void *ptr, size_t old_size, size_t alignment, size_t size) {
	char *new_ptr = aligned_alloc(aligment, size);

	if(new_ptr == NULL) return NULL;

	size_t copy_size = old_size < size ? old_size : size;

	if(ptr != NULL) {
		memcpy(new_ptr, ptr, copy_size);
	}

	free(ptr);

	return new_ptr;
}
```
