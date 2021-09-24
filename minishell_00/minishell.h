#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//#include <readline/readline.h>
//#include <readline/history.h>
#include <fcntl.h>
#include "include/readline/readline.h"
#include "include/readline/history.h"
#include "libft/libft.h"
#include "struct.h"
#include <termios.h>


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


struct termios org_term;//나중에 옮길 수 있음
struct termios new_term;//

//signal.c
void sig_int(int sig_number);
void signal_initialize(void);
void save_input_mode(void);
void set_input_mode(void);
void reset_input_mode(void);


//tokenizer_envp_list.c
int  split_key_value(char *str, char **key, char **value);
void envp_list_initialize(char **envp, t_list **envp_list);
void init_list(t_list **list);
void add_node(t_list *list, char *str);
void delete_list(t_list **list);

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

//parser.c
void    init_parse_list(t_parse_list **list);
void    add_parse_node(t_parse_list *list, t_parse_node *parse_node);
t_parse_node *create_parse_node(t_lex_list *lex_list);
void parse_lexer(t_parse_list *parse_list, t_lex_list *lex_list);

//execution_util.c
int count_lex_node(t_lex_list *lex_list);
char **make_argv(t_parse_node *parse_node, t_list *envp_list);
int is_builtin(char *cmd);
char *make_path(char *cmd, t_list *envp_list);

//execution_run.c
void config_heredoc(t_lex_list *redirection_list);
int    config_redirection(t_lex_list *redirection_list);
void    connect_pipe(int pipefd[2], int io);
int is_heredoc(t_lex_list *redirection_list);
void multi_pipe(t_parse_list *parse_list, t_list *envp_list);
void    execute_line(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list);


//redirection.c
int		redirection_in(char *file);
int		redirection_out(char *file);
int		redirection_double_out(char *file);
void    make_heredoc(char *delimiter);
int     redirection_heredoc(char *delimiter);

//builtin
int ft_export(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
void env(t_list *envp_list);
void pwd();
void cd(t_lex_node *dir, t_list *envp_list);
int ft_unset(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
void    ft_echo(t_parse_node *parse_node);
void ft_exit(t_parse_node *parse_node);

int	ft_strcmp(const char *s1, const char *s2);
int count_node(t_list *list);

//builtin_sorted.c
void    add_sorted_node(t_list *list, char *key, char *value);
void    init_sorted_list(t_list *sorted_list, t_list *envp_list, t_list *shell_list);
void    sort_sorted_list(t_list *sorted_list);
void    print_sorted_list(t_list *sorted_list);


#endif