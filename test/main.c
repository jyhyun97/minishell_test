#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void sig_int(int sig_number)
{
    //^C 안뜨게 하기
    //입력하다 취소하고 엔터누르면 그 다음 줄에 그 입력중이던 값이 출력되는 현상?
    //위 두 현상은 rl_replace_line(); -> 나중에 설치

    //rl_redisplay();
    if (sig_number == SIGINT)
    {
	    write(1, "\nminishell$ ", 12);
    }
    //else if (sig_number == SIGQUIT)
    //{
    //    printf("sig2\n");
    //}
    //else if(sig_number == SIGCHLD)
    //{
    //    printf("sig3\n");
    //}
}

void signal_initialize(void)
{
	signal(SIGINT, sig_int);
	//signal(SIGQUIT, sig_int);
	//signal(SIGCHLD, sig_int);//
}


int main(int argc, char **argv, char **envp)
{
    char *line;
    
    signal_initialize();
    //envp list

    while(1)
    {
        line = readline("minishell$ ");
        //printf("%s\n", line);
        if (line && *line)
            add_history(line);
        free(line);
    }
    return (0);
}