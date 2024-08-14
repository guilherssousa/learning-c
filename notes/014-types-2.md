# Tipos 2

## Inteiros com e sem sinal

`int` é um tipo com sinal. Mas o C também tem tipos sem 
sinal específicos que só podem armazenar vlaores positivos:

```c
int a;
signed int a;
signed a;

unsigned int a;
unsigned c;
```

Você pode usar esses tipos para armazenar valores maiores
do que você conseguiria usando `int`s com sinal.

## Tipos de caractere

O tipo `char` na verdade é um `int`.

- O `sizeof(char) == 1` é uma regra, porém, pela idade da linguagem, a largura de um byte pode variar.
- `char` podem ser com ou sem sinal dependendo do compilador, a menos que você especifique explicitamente.
- A implementação transforma o string literal para um `char` baseado no character set usado.

## Mais tipos de inteiros: `short`, `long` e `long long`

Coisas importantes:

- O header `limits.h` define macros que guardam o valor mínimo e máximo dos inteiros. 
- Sempre use os macros, nunca faça hard-coding.
- Você pode checar a tabela de valores aqui: [Types II: Way more types](https://beej.us/guide/bgc/html/split/types-ii-way-more-types.html)

## Mais floats

Aqui tem um monte de matemáticas sobre o tamanho dos floats.

- Existem macros que especificam o número de casas decimais que você pode usar com floats.

## Convertendo para decimal e vice-versa

Sinceramente, toda uma nerdaiada sobre números que eu particularmente não me interesso.

## Tipos de constantes numéricas

- Para definir um literla com hexadecimal: `0x1A2B`
- Para definir um literla com octal: `012` (sim, começa com 0)
- Algumas extensões não oficiais permitem que você represente binários como `0b101`
- Porém, não tem um _format specififer_ para binários no `printf()`

