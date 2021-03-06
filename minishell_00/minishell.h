#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
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

typedef struct s_gloval
{
    struct termios org_term;
    struct termios new_term;
    int sig_code;
}   t_gloval;

t_gloval g_gloval;

//builtin_export
char *make_key(char *str);
char *make_value(char *str);
int search_list(t_list *list, char *str);
int	ft_strcmp(const char *s1, const char *s2);
int count_node(t_list *list);
char check_export_letter(char *str);
int check_export_format(char *str);
void delete_node(t_list *list);
void print_export(t_list *envp_list, t_list *shell_list);
void export_case_envp(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
void export_case_shell(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
int ft_export(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
int ft_unset(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);

// builtin_sorted
void add_sorted_node(t_list *list, char *key, char *value);
void init_sorted_list(t_list *sorted_list, t_list *envp_list, t_list *shell_list);
void sort_sorted_list(t_list *sorted_list);
void    print_sorted_list(t_list *sorted_list);

// builtin
int ft_echo(t_parse_node *parse_node);
void ft_exit(t_parse_node *parse_node);
int ft_env(t_list *envp_list);
int ft_pwd();
int ft_cd(t_lex_node *dir, t_list *envp_list);

// execution_run
int is_n_option(char *str);
void config_heredoc(t_lex_list *redirection_list);
int config_redirection(t_lex_list *redirection_list);
void connect_pipe(int pipefd[2], int io);
int is_heredoc(t_lex_list *redirection_list);
int execute_builtin(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list);
void multi_pipe(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list);
void execute_line(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list);

// execution_util
int count_lex_node(t_lex_list *lex_list);
char **make_argv(t_parse_node *parse_node, t_list *envp_list);
int is_builtin(char *cmd);
char *make_path(char *cmd, t_list *envp_list);

// lexicalize_token
t_lex_node *create_lex_node(int type, char *value);
void init_lex_list(t_lex_list **list);
void add_lex_node(t_lex_list *list, t_lex_node *lex_node);
void Lexicalize_token(char **tokens, t_lex_list *lex_list);

// parser
void    init_parse_list(t_parse_list **list);
void    add_parse_node(t_parse_list *list, t_parse_node *parse_node);
t_parse_node *create_parse_node(t_lex_list *lex_list);
void parse_lexer(t_parse_list *parse_list, t_lex_list *lex_list);

// redirection
int		redirection_in(char *file);
int		redirection_out(char *file);
int		redirection_double_out(char *file);
void    make_heredoc(char *delimiter);
int     redirection_heredoc(char *delimiter);

// signal
void sig_int(int sig_number);
void signal_initialize(void);
void save_input_mode(void);
void set_input_mode(void);
void reset_input_mode(void);


// tokenizer_divide
int count_tokens(char **tokens);
char **divide_tokens(char **tokens);

// tokenizer_envp_convert
int measure_env_key(char *str);
char *get_env(char *key, t_list *envp_list);
char *new_arr_str(char *arr_str, char *envp_key, t_list *envp_list);
char **convert_env(char **arr, t_list *envp_list);

//tokenizer_envp_list
int  split_key_value(char *str, char **key, char **value);
void envp_list_initialize(char **envp, t_list **envp_list);
void init_list(t_list **list);
void add_node(t_list *list, char *str);
void delete_list(t_list **list);

//tokenizer_parser
char	**arr_free(char **arr);
int skip_quotes(char const *s, char q);
static int cnt_word(char const *s, char c);
static int cnt_letter(char const *s, char c);
static char **arr_fill(char **arr, char const *s, char c);
char	**word_split(char const *s, char c);

//tokenizer_trim
int count_trimed_token(char *token);
char *trim_quote(char *token);
char **trim_tokens(char **tokens);

/*
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
void multi_pipe(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list);
void    execute_line(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list);
int is_n_option(char *str);

//redirection.c
int		redirection_in(char *file);
int		redirection_out(char *file);
int		redirection_double_out(char *file);
void    make_heredoc(char *delimiter);
int     redirection_heredoc(char *delimiter);

//builtin
int ft_export(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
int ft_env(t_list *envp_list);
int ft_pwd();
int ft_cd(t_lex_node *dir, t_list *envp_list);
int ft_unset(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list);
int ft_echo(t_parse_node *parse_node);
void ft_exit(t_parse_node *parse_node);

int	ft_strcmp(const char *s1, const char *s2);
int count_node(t_list *list);

//builtin_sorted.c
void    add_sorted_node(t_list *list, char *key, char *value);
void    init_sorted_list(t_list *sorted_list, t_list *envp_list, t_list *shell_list);
void    sort_sorted_list(t_list *sorted_list);
void    print_sorted_list(t_list *sorted_list);
*/

#endif