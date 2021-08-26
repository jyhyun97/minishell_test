#include "minishell.h"

int  split_key_value(char *str, char **key, char **value)
{
    int i;

    i = 0;
    while (i < ft_strlen(str))
    {
        if (str[i] == '=')
        {
            str[i] = '\0';
            *key = ft_strdup(str);
            if (*key == 0)
                return (0);
            *value = ft_strdup(&str[i + 1]);
            if (*value == 0)
            {
                free(*key);
                return (0);
            }
        }
        i++;
    }
    return (1);
}

void envp_list_initialize(char **envp, t_list **envp_list)
{
    int i;

    init_list(envp_list);
    i = 0;
    while (envp[i] != 0)
    {
        add_node(*envp_list, envp[i]);
        i++;
    }
    (*envp_list)->cur = (*envp_list)->head;   
    // while(envp_list->cur != 0)
    // {
    //     printf("key : %s, value : %s\n", envp_list->cur->key, envp_list->cur->value);
    //     envp_list->cur = envp_list->cur->next;
    // }
}