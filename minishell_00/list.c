//#include <stdlib.h>
#include "list.h"
#include "minishell.h"

void init_list(t_list **list)
{
    *(list) = (t_list *)malloc(sizeof(t_list));
    (*list)->cur = 0;
    (*list)->head = 0;
    (*list)->tail = 0;
}

void add_node(t_list *list, char *str)
{
    t_node *node;

    node = (t_node *)malloc(sizeof(t_node));
    //node->data = data;
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

int count_node(t_list *list)
{
    int num;

    num = 0;
    list->cur = list->head;
    while (list->cur != 0)
    {
        list->cur = list->cur->next;
        num++;
    }
    return (num);
}
