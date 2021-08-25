#include "minishell.h"

int count_trimrd_token(char *token)
{
    int i = 0;
    
    while (token[i] != '\0')
    {
        if (token[i] == '"')
        {
            i++;
            while (token[i] != '"')
                i++;
            i++;
        }
        else if (token[i] == '\'')
        {
            i++;
            while (token[i] != '\'')
                i++;
            i++;
        }
        else
            i++;
    }
    return (i);
}

char *trim_quote(char *token)
{
    /*
        따옴표만 제거하기.
        따옴표 제외한 글자세기.
    */
    char *new_str;
    int i = 0;
    int j = 0;
    new_str = (char *)malloc(sizeof(char) * (count_trimrd_token(token) + 1));
    
    while (token[i] != '\0')
    {
        if (token[i] == '"')
        {
            i++;
            while (token[i] != '"')
            {
                new_str[j] = token[i];
                i++;
                j++;
            }
            i++;
        }
        else if (token[i] == '\'')
        {
            i++;
            while (token[i] != '\'')
            {
                new_str[j] = token[i];
                i++;
                j++;
            }
            i++;
        }
        else
        {
            new_str[j] = token[i];
            i++;
            j++;
        }
    }
    new_str[j] = '\0';
    free(token);
    return (new_str);
}

char **trim_tokens(char **tokens)
{
    /*
        전체 tokens의 내용의 따옴표 제거
    */
    int i = 0;
    while(tokens[i] != NULL)//
    {
        tokens[i] = trim_quote(tokens[i]);
        i++;
    }
    return (tokens);
}
void parse_line(char *line, t_list *envp_list)
{
    char **tokens;
    int i = 0;
    tokens = word_split(line, ' ');

    tokens = convert_env(tokens, envp_list);
    tokens = trim_tokens(tokens);
    
    //따옴표 제거해주는 함수. strtrim ', strtrim " 해주기
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

//  export LDFLAGS="-L/opt/homebrew/opt/readline/lib"
//  export CPPFLAGS="-I/opt/homebrew/opt/readline/include"