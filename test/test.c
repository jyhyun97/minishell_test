/**********************************************************************
* ex_putenv.c                                                         *
* exmple source – print enviroment list                               *
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

    char *env = getenv(argv[1]);
    printf("%s\n",env);
    return 0;
}