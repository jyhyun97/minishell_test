#include "minishell.h"

#define RD_IN_SINGLE 0  //<
#define RD_OUT_SINGLE 1 // >
#define RD_IN_DOUBLE 2  // <<
#define RD_OUT_DOUBLE 3 // >>
#define PIPE 4
#define CMD 5
#define OPTION 6
#define ARGUMENT 7
#define RD_IN_SINGLE_ERR 100
#define RD_OUT_SINGLE_ERR 101
#define RD_IN_DOUBLE_ERR 102  // <<
#define RD_OUT_DOUBLE_ERR 103 // >>
#define PIPE_ERR 104
#define NEW_LINE_ERR 105

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


//렉서 관련 리스트 만들기

//노드 따로 만들기 create node
t_lex_node *create_lex_node(int type, char *value)
{
    t_lex_node *new_node;

    new_node = malloc(sizeof(t_lex_node));
    new_node->type = type;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    return (new_node);
}

void init_lex_list(t_lex_list **list)
{
    *(list) = (t_lex_list *)malloc(sizeof(t_lex_list));
    (*list)->cur = 0;
    (*list)->head = 0;
    (*list)->tail = 0;
}

void add_lex_node(t_lex_list *list, t_lex_node *lex_node)
{
    if (list->head == 0 && list->tail == 0)
    {
        list->head = lex_node;
        list->tail = lex_node;
    }
    else
    {
        list->tail->next = lex_node;
        lex_node->prev = list->tail;
        list->tail = lex_node;
    }
}

void figure_token(char **tokens)
{

}

void Lexicalize_token(char **tokens, t_lex_list *lex_list)
{
    //리스트화
    /*
        <  또는 문자 기준으로
        뒤의 문자 타입 구분 할수 있음.
        타입 구분 해야하면 
        유효한지도 판정이 가능.
    */
    //토큰 타입검사

    int i = 0;
    int flag = 0;
    int type;

    while (tokens[i] != 0)
    {
        if (ft_strncmp(tokens[i], "<<", 2) == 0 || ft_strncmp(tokens[i], ">>", 2) == 0 ||
            tokens[i][0] == '<' || tokens[i][0] == '>' || tokens[i][0] == '|')
        {
            printf("98tokens : %s\n", tokens[i]);
            if (ft_strncmp(tokens[i], "<<", 2) == 0)
                type = RD_IN_DOUBLE;
            else if (ft_strncmp(tokens[i], ">>", 2) == 0)
                type = RD_OUT_DOUBLE;
            else if (tokens[i][0] == '<')
                type = RD_IN_SINGLE;
            else if (tokens[i][0] == '>')
                type = RD_OUT_SINGLE;
            else if (tokens[i][0] == '|')
                type = PIPE;
            i++;
            if (tokens[i] == NULL || ft_strncmp(tokens[i], "<<", 2) == 0 || ft_strncmp(tokens[i], ">>", 2) == 0 ||
            tokens[i][0] == '<' || tokens[i][0] == '>' || tokens[i][0] == '|' || tokens[0][0] == '|')
            {
                
                if (tokens[i] == NULL)
                {
                    add_lex_node(lex_list, create_lex_node(NEW_LINE_ERR, "error"));
                    //value = newline_error;
                    break ;
                }
                //else if (ft_strncmp(tokens[i], "<<", 2) == 0)
                    //value = RD_OUT_DOUBLE_ERR;
                add_lex_node(lex_list, create_lex_node(type, "error"));

                //printf("%d, %s\n", type, "error");//#define >>error <<error <eeror ...
                /*
                    신텍스 에러부분이기떄문에 
                    value = ERROR 
                */
            }
            else
            {
                if (type == PIPE)
                {
                    flag = 0;
                    i--;
                }
                add_lex_node(lex_list, create_lex_node(type, tokens[i]));
                i++;
                //printf("%d, %s\n", type, tokens[i]);
            }
        }
        else
        {
            if (flag == 0)
            {
                type = CMD;
                flag = 1;
            }
            else if (flag == 1)
            {
                if (tokens[i][0] == '-')
                    type = OPTION;
                else
                    type = ARGUMENT;
            }
            add_lex_node(lex_list, create_lex_node(type, tokens[i]));
            // printf("%d, %s\n", type, tokens[i]);
            i++;
        }
    }
    lex_list->cur = lex_list->head;
    while (lex_list->cur != 0)
    {
        printf("type : %d, value : %s\n", lex_list->cur->type, lex_list->cur->value);
        lex_list->cur = lex_list->cur->next;
    }
}

//[리다이렉션s][명령어][리다이렉션s][옵션s][argus][리다이렉션s][파이프]
//리다이렉션 = <,<<,>>,> type. 뒤에 <>|으로 시작하지 않는 문자열 type: < value : 파일이름
//명령어 = 리다이렉션 제외하고 첫번째로 나오는 문자열
//옵션 = 맨 앞에 -으로 시작하는 문자열
//인자 = 명령어, [옵션] 다음에 나오는 문자열
//파이프 = type : pipe, value : |

void parse_line(char *line, t_list *envp_list)
{
    char **tokens;
    // t_lex_node *lex_head;//추후 원한다면 리팩토링
    int i = 0;
    //tokenizing
    tokens = word_split(line, ' ');
    tokens = convert_env(tokens, envp_list);
    tokens = divide_tokens(tokens);
    tokens = trim_tokens(tokens); //순서 바꿈. "ls|env"와 ls|env의 구별이 불가능해서
    while (tokens[i] != 0)
    {
        printf("[%d] : [%s]\n", i, tokens[i]);
        i++;
    }
    t_lex_list *lex_list;
    init_lex_list(&lex_list);
    Lexicalize_token(tokens, lex_list);

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