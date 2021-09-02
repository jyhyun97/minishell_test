#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"

void    connect_pipe(int pipefd[2], int io)
{
    dup2(pipefd[io], io);
    close(pipefd[0]);
    close(pipefd[1]);
}
//gcc *.c -lft -L../libft
int main(int argc, char **argv)
{
    char **new_argv = ft_split("ls -l", ' ');
    char **new_argv1 = ft_split("wc -l -z -z", ' ');
    int count = 4;//프로세스 몇 개 더 만들 건지
    int status;
    pid_t pid = 0;
    int **pipefd;

    pipefd = (int **)malloc(sizeof(int *) * (count));
    for (int i = 0; i < count; i++)
        pipefd[i] = malloc(sizeof(int) * 2);
    while (count > 0 && pid == 0)
    {
        pipe(pipefd[count - 1]);
        pid = fork();
        count--;
    }
    if (pid == 0)//맨 처음 실행되는 자식 프로세스
    {
        printf("i'm a %d child! count : %d\n", pid, count);
        printf("pipefd : %d\n", count);
        connect_pipe(pipefd[count], STDOUT_FILENO);
        //리다이렉션들 처리 하기
        execve("/bin/ls", new_argv, 0);//예시
    }
    else
    {
        waitpid(pid, &status, 0);
        printf("status : %d\n", status);
        printf("i'm a %d parent! and count is %d!!\n", pid, count);
        if (count < 3)//마지막 프로세스 이외의 애들
        {
            printf("pipefd : %d, %d\n", count, count + 1);
            //heardoc이면 파이프 이전에
            connect_pipe(pipefd[count], STDIN_FILENO);
            connect_pipe(pipefd[count + 1], STDOUT_FILENO);
            //리다이렉션 처리
            execve("/usr/bin/wc", new_argv1, 0);
        }
        else//마지막 프로세스~~
        {
            printf("pipefd : %d\n", count);
            connect_pipe(pipefd[count], STDIN_FILENO);
            if (execve("/usr/bin/wc", new_argv1, 0) == -1)
                printf("cmd error\n"); //echo hello | cat < infile
        }
    }
    return (0);
    //부모프로세스 자식프로세스
    //실행 노드를 검사해서 파이프가 true다 이 함수 실행 false다 그냥 빌트인도 다룰 수 있는 뭔가 함수 만들어서 쓰기
    //이것을 함수화 시키기 근데 이건 파이프가 단 하나라도 있다는 걸 전제로 만든 프로그램이라 그걸 고려할 것
    //재귀 없어도 짤 수 있겠다
    //실제로 짤 때는 count값 같은 인덱스나 int pipefd[2]를 파서 노드에 아예 넣어줘도 좋을 듯
}
