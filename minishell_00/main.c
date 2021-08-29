#include "minishell.h"

typedef struct s_parse_node{
    char *cmd;
    
    t_lex_list *option;
    t_lex_list *arg;
    t_lex_list *redirection;
    int pipe;  //파이프 존재시 next노드에 추가 , 굳이 필요 없을듯. 파이프 존재시 1, 미존재 0
    struct s_parse_node *prev;
    struct s_parse_node *next;
} t_parse_node;

typedef struct s_parse_list
{
    t_parse_node *head;
    t_parse_node *cur;
    t_parse_node *tail;
}   t_parse_list;

void    init_parse_list(t_parse_list **list)
{
    *(list) = (t_parse_list *)malloc(sizeof(t_parse_list));
    (*list)->cur = 0;
    (*list)->head = 0;
    (*list)->tail = 0;
}

void    add_parse_node(t_parse_list *list, t_parse_node *parse_node)
{
    if (list->head == 0 && list->tail == 0)
    {
        list->head = parse_node;
        list->tail = parse_node;
    }
    else
    {
        list->tail->next = parse_node;
        parse_node->prev = list->tail;
        list->tail = parse_node;
    }
}

t_parse_node *create_parse_node(t_lex_list *lex_list)
{
    t_parse_node *new_node;
    new_node = malloc(sizeof(t_parse_node));

    init_lex_list(&new_node->option);
    init_lex_list(&new_node->arg);
    init_lex_list(&new_node->redirection);

    while (lex_list->cur != 0 && lex_list->cur->type != PIPE)
    {
        if (lex_list->cur->type == CMD)
            new_node->cmd = ft_strdup(lex_list->cur->value);
        else if (lex_list->cur->type == OPTION)
            add_lex_node(new_node->option, create_lex_node(lex_list->cur->type, ft_strdup(lex_list->cur->value)));
        else if (lex_list->cur->type >= 0 && lex_list->cur->type <= 3)
            add_lex_node(new_node->redirection, create_lex_node(lex_list->cur->type, ft_strdup(lex_list->cur->value)));
        else if (lex_list->cur->type == ARGUMENT)
            add_lex_node(new_node->arg, create_lex_node(lex_list->cur->type, ft_strdup(lex_list->cur->value)));
        new_node->pipe = 0;
        lex_list->cur = lex_list->cur->next;
        if (lex_list->cur == 0)
            break;
    }
    // if (lex_list->cur->type == PIPE)
    //     new_node->pipe = 1;
    return (new_node);
}

void parse_lexer(t_parse_list *parse_list, t_lex_list *lex_list)
{
    lex_list->cur = lex_list->head;
    while (lex_list->cur != 0)
    {
        add_parse_node(parse_list, create_parse_node(lex_list));
        if (lex_list->cur != 0)
            lex_list->cur = lex_list->cur->next;
    }
}

void parse_line(char *line, t_list *envp_list)
{
    char **tokens;
    int i = 0;
    t_lex_list *lex_list;
    t_parse_list *parse_list;
    
    //tokenizing
    line = ft_strtrim(line, " ");
    tokens = word_split(line, ' ');//"> " ">"
    tokens = convert_env(tokens, envp_list);
    tokens = divide_tokens(tokens);
    tokens = trim_tokens(tokens); //순서 바꿈. "ls|env"와 ls|env의 구별이 불가능해서
    while (tokens[i] != 0)
    {
        printf("[%d] : [%s]\n", i, tokens[i]);
        i++;
    }
    //lexicalizing
    init_lex_list(&lex_list);
    Lexicalize_token(tokens, lex_list);
    
    //parsing
    init_parse_list(&parse_list);
    parse_lexer(parse_list, lex_list);

    parse_list->cur = parse_list->head;
    while (parse_list->cur != 0)
    {
        // while(parse_list->cur->)
        // {

        // }
        printf("%s\n", parse_list->cur->cmd);

        parse_list->cur = parse_list->cur->next;
    }//qwe asd zxc qwe 여러번 반복 시 segfalut 발생
    arr_free(tokens);
    /*
        토큰별로 나누어주기
        토큰별 환경변수 처리
        따옴표 제거
        파이프 개수세어서 구조체 리스트 만들기. (key, value)구조체 or 구문 구조체
        keey, value 구분? || 파이프 로 구문 나누기
        구조체 값 넣기
    */
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_list *envp_list;

    //save_input_mode();
    //set_input_mode();
    //signal_initialize();

    envp_list_initialize(envp, &envp_list);

    while (1)
    {
        line = readline("minishell$ "); //

        if (line && *line)
            add_history(line);
        //파서 ft_sprit();
        parse_line(line, envp_list);

        free(line);
        rl_redisplay();
        //rl_replace_line("\n", 0);
    }
    //reset_input_mode();

    return (0);
}