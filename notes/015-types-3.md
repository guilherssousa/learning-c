# Tipos 3: Covnersões

## Conversões de String

### Número para String

- Você pode usar `sprintf()` ou `snprintf()` para isso.

### String para número

- Você pode usar os métodos `atoi`, `atof`, `atol` e `atoll`
- O `a` no começo dos nomes significa ASCII.
- `strtol`, `strtoll`, `strtoul`, `strtoull`, `strtof`, `strtod` e `strtold` podem ser usados para converter strings para floats.

### Convertendo `char`

- Você pode subtrair um `char` por `'0'` para pegar o valor numérico dele.

### Conversões numéricas

- Se você converter zero para bool, será 0. Outros casos, dará 1.
- Se você converter um inteiro para outro tipo de inteiro ele irá fazer wrap.
- No geral, evite converter floats para inteiros.

## Conversões implícitas

### Promovendo inteiros

Se você somar dois `char`, o resultado será promovido para um `int`.

### Conversões aritméticas usuais

- Se você somar int com float num int, ele será convertido para int.
- Se você fizer aritmética de int num float, ele será convertido para um float.

### `void*`

O `void*` pode ser convertido para ou de qualquer tipo de ponteiro.

## Conversões explícitas

### Casting

Você pode passar o novo tipo antes da variável (usado em callbacks, por exemplo)
