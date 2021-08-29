#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

int main(int argc, char **argv)
{



    // const char argv[2][10] = 
    // {
    //     "-b",
    //     "Makefile"
    // };
    char **new_argv = ft_split("minishell -b Makefile", ' ');
    execve("/bin/cat", new_argv, 0);
}