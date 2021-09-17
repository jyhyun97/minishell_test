// #include "../minishell.h"
#include "struct.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define _MAX_PATH 260

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}


void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	i = 0;
	if (dst == NULL && src == NULL)
		return (0);
	tmp_dst = dst;
	tmp_src = (unsigned char *)src;
	while (n > i)
	{
		tmp_dst[i] = tmp_src[i];
		i++;
	}
	return (dst);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*tmp_s1;
	unsigned char	*tmp_s2;

	tmp_s1 = (unsigned char *)s1;
	tmp_s2 = (unsigned char *)s2;
	i = 0;
	while (n > i)
	{
		if (tmp_s1[i] == '\0' || tmp_s2[i] == '\0' || tmp_s1[i] != tmp_s2[i]) //EXC_BAD_ACCESS (code=1, address=0x0)
			return (tmp_s1[i] - tmp_s2[i]);
		i++;
	}
	return (0);
}


char	*ft_strdup(const char *s1)
{
	char	*tmp;
	int		len;

	len = ft_strlen(s1) + 1;
	if ((tmp = malloc(len * sizeof(char))) == NULL)
		return (0);
	ft_memcpy(tmp, s1, len);
	return (tmp);
}

void init_list(t_list **list)
{
    *(list) = (t_list *)malloc(sizeof(t_list));
    (*list)->cur = 0;
    (*list)->head = 0;
    (*list)->tail = 0;
}

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

void add_node(t_list *list, char *str)
{
    t_node *node;

    node = (t_node *)malloc(sizeof(t_node));
    split_key_value(str, &node->key, &node->value);
    node->prev = 0;
    node->next = 0;
    if (list->head == 0 && list->tail == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
}

void delete_list(t_list **list)
{
    t_node *tmp;

    (*list)->cur = (*list)->head;
    while ((*list)->cur != 0)
    {
        tmp = (*list)->cur->next;
        free((*list)->cur);
        (*list)->cur = tmp;
    }
    free(*list);
}


void envp_list_initialize(char **envp, t_list **envp_list)
{
    int i;
    char *env_tmp = ft_strdup("?=0");

    init_list(envp_list);
    add_node(*envp_list, env_tmp);
    free(env_tmp);
    i = 0;
    while (envp[i] != 0)
    {
        add_node(*envp_list, envp[i]);
        i++;
    }
    (*envp_list)->cur = (*envp_list)->head;
}


void pwd()
{
    char path[1024];
    printf("%s\n", getcwd(path, 1024));
}

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

void cd(char *dir, t_list *envp_list)
{
    int nResult;
    if (dir == 0)
        nResult = chdir( "../" );
    nResult = chdir( dir );
    char path[1024];

	if( nResult == -1 )
	{
		printf( "no such file or directory: %s\n", dir);
		return ;
	}
	envp_list->cur = envp_list->head;
    while (envp_list->cur != 0)
    {
		if(ft_strncmp(envp_list->cur->key, "PWD", 3) == 0)
		{
			envp_list->cur->value = NULL;
			free(envp_list->cur->value);
			envp_list->cur->value = ft_strdup(getcwd(path, 1024));
			return;
		}
		envp_list->cur = envp_list->cur->next;
    }
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_list *envp_list;
	
	// printf("========env=========\n");
	envp_list_initialize(envp, &envp_list);
    // env(envp_list);
	// printf("========pwd=========\n");
	// pwd();
	// printf("========cd - pwd=========\n");
	cd("/", envp_list);
	pwd();
	printf("========env=========\n");
    env(envp_list);
	return 0;
}
