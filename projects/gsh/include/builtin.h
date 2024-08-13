#ifndef GSH_BUILTIN_H
#define GSH_BUILTIN_H

extern char *builtin_str[];
extern int (*builtin_func[])(char **);

// Function declarations for builtin shell commands
int gsh_cd(char **args);
int gsh_help(char **args);
int gsh_exit(char **args);

int gsh_num_builtins();

#endif
