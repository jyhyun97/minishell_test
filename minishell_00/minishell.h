#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
// #include "include/readline/readline.h"
// #include "include/readline/history.h"
#include "libft/libft.h"

#include "list.h"


#define RD_IN_SINGLE 0  //<
#define RD_OUT_SINGLE 1 // >
#define RD_IN_DOUBLE 2  // <<
#define RD_OUT_DOUBLE 3 // >>
#define PIPE 4
#define CMD 5
#define OPTION 6
#define ARGUMENT 7
#define RD_IN_SINGLE_ERR 100
#define RD_OUT_SINGLE_ERR 101
#define RD_IN_DOUBLE_ERR 102
#define RD_OUT_DOUBLE_ERR 103
#define PIPE_ERR 104
#define NEW_LINE_ERR 105

//signal.c
void sig_int(int sig_number);
void signal_initialize(void);

//envp_list.c
int  split_key_value(char *str, char **key, char **value);
void envp_list_initialize(char **envp, t_list **envp_list);

//tokenizer_trim.c
char **trim_tokens(char **tokens);

//envp_convert.c
char **convert_env(char **arr, t_list *envp_list);

//tokenizer_divide
char **divide_tokens(char **tokens);

//lexicalize token
void init_lex_list(t_lex_list **list);
void add_lex_node(t_lex_list *list, t_lex_node *lex_node);
void Lexicalize_token(char **tokens, t_lex_list *lex_list);

t_lex_node *create_lex_node(int type, char *value);

//util.c
size_t ft_strlen(const char *s);
char *ft_strdup(const char *s1);

//parser.c
int skip_quotes(char const *s, char q);
char	**word_split(char const *s, char c);
char	**arr_free(char **arr);
#endif