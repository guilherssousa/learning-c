# Hello World

- Tudo entre os digrafos `/*` e `*/` s'ao comentarios e serao completamente ignorados pelo compilador.
- O `#include` avisa ao preprocessador para puxar os conteudos de outro arquivo e inserir no codigo.

Existem duas fases de compilacao, o preprocessador e o compilador.
Tudo que comecar com um jogo da velha (#), e algo que o preprocessador trabalha antes que o compilador seja chamado.

Depois disso, o compilador pega o codigo e transforma em codigo de maquina que a CPU entende.

O que e o `<stdio.h>`? E chamado de header file, ele importa as
funcionalidades de "Standard I/O", no nosso programa demo precisaremos do `printf()`.
Se tentassemos usar o `printf()` sem o `#include <stdio.h>`, o compilador iria reclamar sobre isso.

A funcao `main` e especial de varias formas, ela sera chamada automaticamente quando
o programa comecar a ser executado.

Para compilar o codigo:

```bash
gcc -o hello 002-hello-world.c
```

## Detalhes da compilacao

Compilacao e o processo de pegar o codigo fonte e transformar em um programa
que seu sistema operacional consegue executar.

Quando rodamos `gcc -o hello 002-hello-world.c`, o `-q` indica qual e o arquivo
de saida do compilador.

Voce tambem pode compilar multiplos arquivos em um unico passando para o comando.

```bash
gcc -o awesomegame ui.c characters.c npc.c items.c
```
