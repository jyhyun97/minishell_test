#include "minishell.h"

void env(t_list *envp_list)
{
 	envp_list->cur = envp_list->head;
    while (envp_list->cur != 0)
    {
		if(envp_list->cur->value[0] == '?')
            envp_list->cur = envp_list->cur->next;
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

