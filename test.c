#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **argv, char **envp)
{
    char *line;
    
    while(1)
    {
        line = readline("minishell$ ");
        printf("%s\n", line);
        if (line && *line)
            add_history(line);
        free(line);
    }
    return (0);
}