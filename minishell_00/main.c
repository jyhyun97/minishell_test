#include "minishell.h"

void delete_lex_list(t_lex_list **lex_list)
{
    t_lex_node *tmp;

    (*lex_list)->cur = (*lex_list)->head;
    while ((*lex_list)->cur != 0)
    {
        free((*lex_list)->cur->value);
        tmp = (*lex_list)->cur->next;
        free((*lex_list)->cur);
        (*lex_list)->cur = tmp;
    }
    free(*lex_list);
    *lex_list = NULL;
}

void delete_parse_list(t_parse_list **parse_list)
{
    int i = 0;
    t_parse_node *tmp;

    (*parse_list)->cur = (*parse_list)->head;
    while ((*parse_list)->cur != 0)
    {
        i++;
        if ((*parse_list)->cur->cmd != 0)
        {
            free((*parse_list)->cur->cmd);
            (*parse_list)->cur->cmd = NULL;
        }
        if ((*parse_list)->cur->option != 0)
            delete_lex_list(&(*parse_list)->cur->option);
        if ((*parse_list)->cur->arg != 0)
            delete_lex_list(&(*parse_list)->cur->arg);
        if ((*parse_list)->cur->redirection != 0)
            delete_lex_list(&(*parse_list)->cur->redirection);
        tmp = (*parse_list)->cur->next;
        free((*parse_list)->cur);
        ((*parse_list)->cur) = NULL;
        (*parse_list)->cur = tmp;
    }
    free(*parse_list);
    (*parse_list) = NULL;


}

t_parse_list *parse_line(char *line, t_list *envp_list)
{
    char **tokens;
    int i = 0;
    char *trimed_line;
    t_lex_list *lex_list;
    t_parse_list *parse_list;
    
    //tokenizing
    if (line == 0)
        return (0);
    trimed_line = ft_strtrim(line, " ");  //???문제 있어요.
    free(line);
    tokens = word_split(trimed_line, ' ');
    free(trimed_line);
    tokens = convert_env(tokens, envp_list);
    tokens = divide_tokens(tokens); //???
    tokens = trim_tokens(tokens); // ???
    while (tokens[i] != 0)
    {
        printf("[%d] : [%s]\n", i, tokens[i]);
        i++;
    }
    //lexicalizing
    init_lex_list(&lex_list);
    Lexicalize_token(tokens, lex_list);
    //if syntax error check == error
    //{
        //arr_free(tokens) //delete_lex_list
        //return (0);
    //}
    //---token_free---
    arr_free(tokens);

    lex_list->cur = lex_list->head;
    while (lex_list->cur != 0)
    {
        printf("type %d, value %s\n", lex_list->cur->type, lex_list->cur->value);
        lex_list->cur = lex_list->cur->next;
    }

    //parsing
    init_parse_list(&parse_list);
    parse_lexer(parse_list, lex_list);
    //---lex_free---
    delete_lex_list(&lex_list);

    // parse_list->cur = parse_list->head;
    // while (parse_list->cur != 0)
    // {
    //     // printf("%s, %d\n", parse_list->cur->cmd, parse_list->cur->index);
    //     parse_list->cur = parse_list->cur->next;
    // }
    return (parse_list);
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_list *envp_list;
    t_parse_list *parse_list;


    //save_input_mode();
    //set_input_mode();
    //signal_initialize();

    envp_list_initialize(envp, &envp_list);

    while (1)
    {
        line = readline("minishell$ "); //공뱍들어왔을떄 처리
        if (line && *line)
            add_history(line);
        parse_list = parse_line(line, envp_list);
        //실행부
        execute_line(parse_list, envp_list);
        //프리
        delete_parse_list(&parse_list);
        // rl_redisplay();
        // rl_replace_line("\n", 0);
    }
    //reset_input_mode();
    delete_list(&envp_list);
    return (0);
}
