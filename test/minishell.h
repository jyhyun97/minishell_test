#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//#include <readline/readline.h>
//#include <readline/history.h>
#include "include/readline/readline.h"
#include "include/readline/history.h"
#include "list.h"

//signal.c
void sig_int(int sig_number);
void signal_initialize(void);

//envp_list.c
int  split_key_value(char *str, char **key, char **value);
void envp_list_initialize(char **envp, t_list **envp_list);

//util.c
size_t ft_strlen(const char *s);
char *ft_strdup(const char *s1);


#endif