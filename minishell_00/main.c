#include "minishell.h"

void parse_line(char *line, t_list *envp_list)
{
    char **arr;
    int i = 0;
    arr = word_split(line, ' ');

    //  
    // while (arr[i] != 0)
	// {
	// 	printf("[%d] : [%s]\n", i, arr[i]);
	// 	i++;
	// }
    arr_free(arr);
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
        rl_replace_line("\n", 0);
    }
  	//reset_input_mode();

    return (0);
}

//  export LDFLAGS="-L/opt/homebrew/opt/readline/lib"
//  export CPPFLAGS="-I/opt/homebrew/opt/readline/include"