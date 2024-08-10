# Arrays

Arrays em C são apenas açúcar sintático.

Quando você declara um array, você precisa dar um tamanho para ele.
E ele precisa ser fixo.

No exemplo do `006-arrays.c`, fizemos um array com 4 `float`s. O valor nos
colchetes nos deixa saber disso.

Nas linhas subsequentes, nós acessamos os valoers no array, definindo ou
buscando eles, novamente, com colchetes.

## Buscando o tamanho de um Array.

Você não consegue~ish. O C não mantém controle dessa informação, você precisa
gerenciar isso separadamente com outra variável.

Existem algumas circunstâncias onde você pode, mas tem um trique para buscar
o número de elementos em um array no escopo onde o array é declarado. Mas,
geralmente, isso não vai funcionar do jeito que você pensa quando você passar
o array para uma função.

Vamos olhar para esse truque: A ideia é que você pegue o `sizeof` do array,
e divide pelo tamanho de cada elemento para pegar a largura. Por exemplo,
se um `int` é 4 bytes e o array é 32 bytes, então pode ter espaço para
32/4 = 8 ints ali.

Se é um array de `char`s, então o `sizeof` do array é o número de elementos,
já que o `sizeof(char)` é 1. Para qualquer outra coisa, você precisa dividir
pelo tamanho de cada elemento.

Mas esse truque só funciona no escopo em que o array foi definido. Se você
passa o array para uma função, isso não funciona, mesmo se você deixar bem 
explícito na assinatura da função.

Isso é porque quando você passa arrays para funções, você está passando apenas
um ponteiro para o primeiro elemento, e é isso que o `sizeof` vai medir.

Uma última coisa que você pode fazer com `sizeof` e arrays é pegar o tamanho
de um array de número fixo de elementos sem declarar o array. É como
você consegue o tamanho de um `int` com um `sizeof(int)`.

Por exemplo, para checar quantos bytes serão necessários para um array de 48
`double`s, você pode fazer isso:

```c
sizeof(double[48]);
```

## Inicializadores de Array

Você pode inicializar um array com constantes:

```c
#include <stdio.h>

int main(void) {
    int i;
    int a[5] = {22, 37, 3490, 18, 95};

    for (i = 0; i < 5; i++) {
        printf("%d\n", a[i]);
    }
}
```

Você nunca deve ter mais itens no seu inicializador do que
tem de espaço no seu array, ou se não o compilador irá ficar doido.

Porém, você pode ter menos itens no seu inicializador do que tem de espaço
no seu array. O resto do espaço será automaticamente inicializado com zero.

Um shortcut comum de se ver em um inicializador é quando você quer definir
um array inteiro como zero:

```c
int a[100] = {0};

You can set specific array elements in the initializer, as well, by specifying an index for the value! When you do this, C will happily keep initializing subsequent values for you until the initializer runs out, filling everything else with 0.

To do this, put the index in square brackets with an = after, and then set the value.

Here’s an example where we build an array:

int a[10] = {0, 11, 22, [5]=55, 66, 77};

Because we listed index 5 as the start for 55, the resulting data in the array is:

0 11 22 0 0 55 66 77 0 0

You can put simple constant expressions in there, as well.

#define COUNT 5

int a[COUNT] = {[COUNT-3]=3, 2, 1};
```

## Out of bounds 

O C não te impede de acessar arrays fora dos limites. E nem vai te avisar sobre.

Vamos repetir o exemplo de cima e printar o array, ele apenas tem 5 elementos,
mas vamos tentar printar 10 e ver o que acontece:

```c
#include <stdio.h>

int main(void) {
    int i;
    int a[5] = {22, 37, 3490, 18, 95};

    for (i = 0; i < 10; i++) {
        printf("%d\n", a[i]);
    }
}
```

Rodar isso retorna:

```
22
37
3490
18
95
32765
1847052032
1780534144
-56487472
21890
```

O que é isso? Printar fora do fim de um array resulta no que os desenvolvedores C 
chamam de "comportamento inesperado". Que geralmente significa "você fez algo errado
e tudo pode acontecer durante a execução do seu programa."

"tudo" significa coisas como achar zeros, achar números lixo ou crashar.
A especificação do C diz que nessa circunstância o compilador pode emitir código
que não faz nada.
