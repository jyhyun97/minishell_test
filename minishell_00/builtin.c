#include "minishell.h"

void ft_exit(t_parse_node *parse_node)
{
    int arg_cout = count_lex_node(parse_node->arg);
    int i;

    i = 0;
    if (parse_node->arg->head == 0)
        exit (0);
    if (arg_cout > 1)
    {
        printf("exit\nexit: too many arguments\n");
        exit(1);
    }
    while(i < ft_strlen(parse_node->arg->head->value))
    {
        if (ft_isdigit(parse_node->arg->head->value[i]) == 0)
        {
            printf("exit\nexit: %s numeric argument required\n", parse_node->arg->head->value);
            exit(255);
        }
        i++;
    }
    exit(ft_atoi(parse_node->arg->head->value)); // 반환값은?
}

void env(t_list *envp_list)
{
 	envp_list->cur = envp_list->head;
    while (envp_list->cur != 0)
    {
		if (ft_strncmp(envp_list->cur->key, "?", 2) == 0)
            printf("");
        else
            printf("%s=%s\n", envp_list->cur->key, envp_list->cur->value);
        envp_list->cur = envp_list->cur->next;
    }
}

void pwd()
{
    char path[1024];
    printf("%s\n", getcwd(path, 1024));
}

void cd(t_lex_node *dir, t_list *envp_list)
{
    int nResult;
    char path[1024];
    if (dir == 0){
        while (envp_list->cur != 0)
        {
            if(ft_strncmp(envp_list->cur->key, "HOME", 5) == 0)
                break;
            envp_list->cur = envp_list->cur->next;
        }
        nResult = chdir(envp_list->cur->value);
        if( nResult == -1 )
        {
            printf( "no such file or directory: %s\n", dir->value);
            return ;
        }
        envp_list->cur = envp_list->head;
        while (envp_list->cur != 0)
        {
            if(ft_strncmp(envp_list->cur->key, "PWD", 4) == 0)
            {
                envp_list->cur->value = NULL;
                free(envp_list->cur->value);
                envp_list->cur->value = ft_strdup(getcwd(path, 1024));
                return;
            }
            envp_list->cur = envp_list->cur->next;
        }
    }
    else
    {
        nResult = chdir( dir->value );
        if( nResult == -1 )
        {
            printf( "no such file or directory: %s\n", dir->value);
            return ;
        }
        envp_list->cur = envp_list->head;
        while (envp_list->cur != 0)
        {
            if(ft_strncmp(envp_list->cur->key, "PWD", 4) == 0)
            {
                envp_list->cur->value = NULL;
                free(envp_list->cur->value);
                envp_list->cur->value = ft_strdup(getcwd(path, 1024));
                return;
            }
            envp_list->cur = envp_list->cur->next;
        }
    }
}
