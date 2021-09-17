#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int count_node(t_list *list)
{
    int num;

    num = 0;
    list->cur = list->head;
    while (list->cur != 0)
    {
        num++;
        list->cur = list->cur->next;
    }
    return (num);
}

char check_export_letter(char *str)
{
    int i;

    i = 0;
    if (str[i] == '?' || str[i] == '=' || str[i] == '$')
        return (str[i]);
    i++;
    while (str[i] != '\0')
    {
        if (str[i] == '?' || str[i] == '$')
            return (str[i]);
        i++;
    }
    return (0);
}

int check_export_format(char *str)
{
    int i = 0;
    if (str[i] == '=')//처음이 =이 아닌지 확인
        return (1);
    i++;
    while (str[i] != '\0' && str[i] != '=')
        i++;
    if (str[i] != '=')//중간이 =인지 확인
        return (1);
    i++;
    if (str[i] == '\0')//바로 다음 글자가 널문자인지 확인
        return (1);
    return (0);
}

int ft_export(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list)
{
    t_list *sorted_list;
    int rtn;

    init_list(&sorted_list);
    init_sorted_list(sorted_list, envp_list, shell_list);
    sort_sorted_list(sorted_list);
    rtn = 0;

    parse_node->arg->cur = parse_node->arg->head;
    if (parse_node->arg->cur == 0)
        print_sorted_list(sorted_list);  
    else
    {
        parse_node->arg->cur = parse_node->arg->head;
        while (parse_node->arg->cur != 0)
        {
            if (check_export_letter(parse_node->arg->cur->value) != 0)
            {
                printf("export: `%s': not a valid identifier\n", parse_node->arg->cur->value);
                rtn = 1;
            }
            else
            {
                if (check_export_format(parse_node->arg->cur->value) == 0)
                    //else if (aa가 기존에 있는 값이다)
                        //수정만 해준다. 
                    add_node(envp_list, parse_node->arg->cur->value);
                else
                    add_sorted_node(shell_list, parse_node->arg->cur->value, 0);
            }
            parse_node->arg->cur = parse_node->arg->cur->next;
        }
    }
    delete_list(&sorted_list);
    return (rtn);
}

// int ft_unset(t_parse_node *parse_node, t_list *envp_list, t_list *shell_list)
// {
//     //unset export, envp 리스트 둘 다 지워줌.
//     //없는 변수 넣어도 동작은 함
//     ////= . $ 이런 애들은 not a valid identifier출력 후 1반환하기.
// }

//int ft_exit()