# Funções

Um exemplo de declaração de função em C:

```c
#include <stdio.h>

int plus_one(int n) {
    return n + 1;
}
```

O int antes do `plus_one` indica o tipo do retorno.
O `void` na declaração do `main` indica que ele nao recebe argumentos.

## Passando por valor

Quando você passa um argumento para uma função, uma cópia do argumento é feita
e é armazenada no parâmetro correspondente.

Se o argumento for uma variável, uma cópia do valor da variável é feita e
armazenada no parâmetro.

Nenhum nome bonitinho vai te distrair do fato que tudo que voce passar para uma função
sem excessão é copiado para o parametro correspondente e a função opera naquela cópia,
não importa o que.

## Protótipos de Função

Você pode avisar ao compilador que você usará uma função de um certo tipo que terá
uma certa lista de parametros. Dessa forma a função pode ser definida em qualquer
lugar, mesmo num arquivo diferente, desde que o prototipo da função seja declarado
antes que você chame a função.
