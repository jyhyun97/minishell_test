#ifndef LIST_H
# define LIST_H

typedef struct s_node
{
    char *key; //제일 처음에 나오는 '='를 기준으로 왼쪽
    char *value; //오른쪽
    struct s_node *prev;
    struct s_node *next;
}   t_node;

typedef struct s_list
{
    t_node *head;
    t_node *cur;
    t_node *tail;
}   t_list;

typedef struct s_lex_node
{
    int type; // 타입 종류는?
    char *value;
    // int error;
    struct s_lex_node *prev;
    struct s_lex_node *next;
} t_lex_node;


typedef struct s_lex_list
{
    t_lex_node *head;
    t_lex_node *cur;
    t_lex_node *tail;
}   t_lex_list;


//렉서
//파서는 자료구조

//
//



// struct general {
//     t_list *envp_list;
//     t_lexer *lexer_head; 
// }
//free.. 



//list.c
void init_list(t_list **list);
void add_node(t_list *list, char *str);
void delete_list(t_list **list);
int count_node(t_list *list);

#endif