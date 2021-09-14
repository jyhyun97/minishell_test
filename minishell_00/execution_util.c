#include "minishell.h"

int count_lex_node(t_lex_list *lex_list)
{
    int num;

    num = 0;
    lex_list->cur = lex_list->head;
    while (lex_list->cur != 0)
    {
        lex_list->cur = lex_list->cur->next;
        num++;
    }
    return (num);
}

char **make_argv(t_parse_node *parse_node, t_list *envp_list)
{
    char **new_argv;
    int cnt_all_node;
    int i;

    cnt_all_node = count_lex_node(parse_node->option) + count_lex_node(parse_node->arg);
    new_argv = (char **)malloc(sizeof(char *) * (cnt_all_node + 2));
    i = 0;
    parse_node->option->cur = parse_node->option->head;
    new_argv[i] = parse_node->cmd; //cmd , minishell 파일이름..
    i++;
    while (parse_node->option->cur != 0)
    {
        new_argv[i] = parse_node->option->cur->value;
        parse_node->option->cur = parse_node->option->cur->next;
        i++;
    }
    parse_node->arg->cur = parse_node->arg->head;
    while (parse_node->arg->cur != 0)
    {
        new_argv[i] = parse_node->arg->cur->value;
        parse_node->arg->cur = parse_node->arg->cur->next;
        i++;
    }
    new_argv[i] = 0;

    return (new_argv);
}

int is_builtin(char *cmd)
{
    if (cmd == NULL)
        return (0);
    if (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "echo", 4) == 0 ||
        ft_strncmp(cmd, "export", 6) == 0 || ft_strncmp(cmd, "unset", 5) == 0 ||
        ft_strncmp(cmd, "env", 3) == 0 || ft_strncmp(cmd, "pwd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 3) == 0)
        return (0);//빌트인
    else
        return (1);//빌트인 아님
}

char *make_path(char *cmd, t_list *envp_list)
{
    char **bins;
    char *new_path;
    char *tmp;
    int i;

    envp_list->cur = envp_list->head;
    while (envp_list->cur != 0)
    {
        if (ft_strncmp(envp_list->cur->key, "PATH", 4) == 0)
            break;
        envp_list->cur = envp_list->cur->next;
    }
    if (envp_list->cur == 0)
        return (0);
    bins = ft_split(envp_list->cur->value, ':');
    i = 0;
    while (bins[i] != 0)
    {
        tmp = ft_strjoin(bins[i], "/");
        new_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(new_path, R_OK) != -1)
        {
            arr_free(bins);
            return (new_path);
        }
        i++;
    }
    arr_free(bins);
    return (0);
}