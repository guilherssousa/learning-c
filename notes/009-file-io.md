# Input/Output de arquivos

## O dado do tipo `FILE*`

Quando fazemos qualquer tipo de I/O em C, fazemos isso com um pedaço de dado
que você recebe no formato do arquivo `FORM*`. Esse `FILE*` armazena todas 
as informações necessárias para se comunicar com o subsistema de `I/O` sobre
qual arquivoc você tem aberto, onde você está no arquivo e etc.

A especificação se refere a esses como `streams`.

| nome do `FILE*` | Descrição |
| --------------- | --------- |
| `stdin` | Standard Input, geralmente o teclado |
| `stdout` | Standard Output, geralmente a tela |
| `stderr` | Standard Error, geralmente a tela por padrão, também |

Por exemplo, essas duas chamadas são equivalentes:

```c
printf("Hello, world!\n");
fprintf(stdout, "Hello, world!\n"); // printf to a file
```

## Lendo arquivos de texto

Streams são categorizadas de duas formas diferentes: texto e binário.

Streams de texto permitem fazer tradução dos dados, como traduzir `newlines`
para as suas representações. Arquvios de texto são logicamente uma sequência
de linhas separada por `newline`s.

A regra geral é que se você pode editar o arquivo num editor de texto, então
é um arquivo de texto. Se não, é um binário.

```c
#include <stdio.h>

int main(void) {
  FILE *fp;

  fp = fopen("./demo/hello.txt", "r"); // <- Abrir o arquivo para leitura

  int c = fgetc(fp);
  printf("%c\n", c);

  fclose(fp);
}
```

Primeiro, abrimos o arquivo com o `fopen()`, que retorna o arquivo `FILE*`
para que possamos usar depois. (Se o arquivo não existir, ele irá retornar NULL)

Depois, passamos a flag `r`, que indica que é "open a text stream for reading".

Por último, fechamos a stream quando terminamos de usar ela.

## EOF

Existe um caractere especial definido como um macro chamado de `EOF`. É isso que
o `fgetc()` irá retornar quando o fim do arquivo for atingido e você tentou ler
outro caractere.

## Lendo uma linha por vez

Usamos o `fgets()` para ler uma linha inteira de uma vez. Como argumento,
ele usa um pointer para um buffer `char` para armazenar os bytes, um número
máximo de bytes para ler e um `FILE*` para ler. Ele retorna `NULL` em caso de
EOL ou erro.

## Input formatado

Vamos usar o `fscanf()` para fazer um input formatado do conteúdo do arquivo.

Abaixo, a assinatura da função `fscanf()`:

```c
int fscanf(FILE *file_stream, char *format, ...any destinatarios);
```

## Escrevendo arquivos de texto

```c
#include <stdio.h>

int main(void)
{
    FILE *fp;
    int x = 32;

    fp = fopen("output.txt", "w");

    fputc('B', fp);
    fputc('\n', fp);   // newline
    fprintf(fp, "x = %d\n", x);
    fputs("Hello, world!\n", fp);

    fclose(fp);
}
```

## Binary File I/O

A única diferença entre abrir um arquivo binário pra um de texto é a flag `b`.
Como streams de bytes podem conter `NUL`, é raro usar funções como `fprintf()`
nesses tipos de arquivos. Os mais comuns são `fread()` e `fwrite()`.

## `struct` e number caveats

Os compiladores podem usar padding em structs e alterar a ordem de números
dependendo do compilador e do `endianess` da arquitetura.

Para resolver esse problema, o comum é serializar os dados para um formato
que possamos controlar e interpretar. Felizmente, isso é um problema que já
foi resolvido, como por exemplo, com `protobuf` do Google.
