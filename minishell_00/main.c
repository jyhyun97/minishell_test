#include "minishell.h"


typedef struct s_lex_node
{
    int type;  // 타입 종류는?
    char *value;
    struct s_lexical *prev;
    struct s_lexical *next;
}   t_lex_node;

// //노드 따로 만들기 create node
// t_lex_node create_lexical_node(int type, char *value)
// {
//     t_lex_node new_node;

//     return (new_node);
// }

// //붙여주는 것 따로 add node
// void add_lexical_node(t_lex_node *cur_node, t_lex_node *next_node)
// {

// }


void figure_token(char **tokens)
{
    
}

void Lexicalize_token(char **tokens)
{
    //리스트화
    /*
        <  또는 문자 기준으로
        뒤의 문자 타입 구분 할수 있음.
        타입 구분 해야하면 
        유효한지도 판정이 가능.
    */
    //토큰 타입검사
    

}
/*
    int cmd_flag//
    while(tokens[i])
    {
        //if 리다이렉션?
            add_node(i, i + 1)//add_node(key, value)
            i += 2;
        //else
            if (명령어?)//flag 필요?
                add_node(cmd, i)
                cmd_flag = 1;
            else if (옵션?)
                add_node(opt, i)
            else if (인자?)
                add_node(arg)
            else if (파이프?)
                add_node(pipe, i)
            i++;
    }
*/

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
    tokens = trim_tokens(tokens);//순서 바꿈. "ls|env"와 ls|env의 구별이 불가능해서
    
    //lexicalization
    //tokens[i] 돌면서 type value 나누고
    //
    //
    //add lexical node(cur node, create lexical node)

    //리스트화
    /*
        <  또는 문자 기준으로
        뒤의 문자 타입 구분 할수 있음.
        타입 구분 해야하면 
        유효한지도 판정이 가능.
        
    */


    while (tokens[i] != 0)
	{
		printf("[%d] : [%s]\n", i, tokens[i]);
		i++;
	}


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
        line = readline("minishell$ ");//
        
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