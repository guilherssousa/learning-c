#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/builtin.h"

void gsh_error(char *message) {
  fprintf(stderr, "gsh: %s\n", message);
  exit(EXIT_FAILURE);
}

#define GSH_RL_BUFSIZE 1024
char *gsh_read_line() {
  int bufsize = GSH_RL_BUFSIZE;
  int pos = 0;
  char *buf = malloc(sizeof(char) * bufsize);
  int c;

  if (!buf) {
    gsh_error("allocation error");
  }

  while (1) {
    // read character
    c = getchar();

    // Check if its the end of line, if not, add to buffer
    if (c == EOF || c == '\n') {
      buf[pos] = '\0';
      return buf;
    } else {
      buf[pos] = c;
    }
    pos++;
  }

  if (pos >= bufsize) {
    bufsize += GSH_RL_BUFSIZE;
    buf = realloc(buf, bufsize);
    if (!buf) {
      gsh_error("allocation error");
    }
  }
}

#define GSH_TOK_BUFSIZE 64
#define GSH_TOK_DELIM " \t\r\n\a"
char **gsh_split_line(char *line) {
  int bufsize = GSH_TOK_BUFSIZE, pos = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    gsh_error("allocation error");
  }

  token = strtok(line, GSH_TOK_DELIM);
  while (token != NULL) {
    tokens[pos] = token;
    pos++;

    if (pos >= bufsize) {
      bufsize += GSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        gsh_error("allocation error");
      }
    }

    // add null-terminate
    token = strtok(NULL, GSH_TOK_DELIM);
  }
  tokens[pos] = NULL;

  return tokens;
}

int gsh_launch(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // child process
    if (execvp(args[0], args) == -1) {
      perror("gsh"); // <- TODO: understand
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // an error has occurred during fork
    // No error-handling beyond telling user and letting
    // them decicde if they need to quit.
    perror("gsh");
  } else {
    // parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int gsh_execute(char **args) {
  int i;

  if (args[0] == NULL) {
    // user simply pressed enter.
    return 1;
  }

  for (i = 0; i < gsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return gsh_launch(args);
}

void gsh_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = gsh_read_line();
    args = gsh_split_line(line);
    status = gsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(void) {
  gsh_loop();

  return EXIT_SUCCESS;
}
