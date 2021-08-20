#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "test/include/readline/readline.h"
#include "test/include/readline/history.h"
#include "libft/libft.h"

/*
	1. 공백 기준으로 split(따옴표, 쌍따옴표 안에 공백있을 시는 무시)
	2. 따옴표 없애주기, 환경변수 처리

	3. split 2차원 배열을 토큰 리스트에 추가  key, value
*/

char **convert_env(char **arr, )
{
    
}


int main(void)
{
	char *line;

    while (1)
    {
        line = readline("minishell$ ");
		if (line && *line)
            add_history(line);
        parse_line(line);
        free(line);
        rl_redisplay();
        rl_replace_line("\n", 0);
    }
	return (0);
}

//  export LDFLAGS="-L/opt/homebrew/opt/readline/lib"
//  export CPPFLAGS="-I/opt/homebrew/opt/readline/include"
