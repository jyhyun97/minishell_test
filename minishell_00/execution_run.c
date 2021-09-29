#include "minishell.h"

int is_n_option(char *str)
{
    //맨 처음이 -이고, 그 다음부터 널문자까지 계속 n인가요? 0 아니면 1
    int i;

    i = 0;
    if (str[i] != '-') //첫 글자가 -이어야 함
        return (1);
    i++;
    if (str[i] == '\0') //혹시 "-" 만 있고 뒤에 아무것도 없으면 안됨
        return (1);
    while (str[i] != '\0')
    {
        if (str[i] != 'n') //만약 중간에 ㄴn이 아닌 문자가 있다면 안됨
            return (1);
        i++;
    }
    return (0); //여기까지 통과하면 올바른 -n 옵션
}

void ft_echo(t_parse_node *parse_node)
{
    int flag_n;
    flag_n = 0;

    //-n 옵션은 어떻게 할까? 유효하지 않은 옵션이 동시에 들어오는 경우는?

    //옵션 처리 이쪽에서
    //유효한 n 옵션은 플래그로 저장하고 쭉 넘겨서, 유효하지 않은 n옵션이 등장한 순간부터 출력하면 됨
    //-nnnnn -n -n -n 유효 while : flag on
    //-n-n-n -x -z 무효 while : printf
    parse_node->option->cur = parse_node->option->head;
    while (parse_node->option->cur != 0 && is_n_option(parse_node->option->cur->value) == 0)
    {
        flag_n = 1;
        parse_node->option->cur = parse_node->option->cur->next;
    }
    //무효 옵션 출력
    while (parse_node->option->cur != 0)
    {
        printf("%s ", parse_node->option->cur->value);
        parse_node->option->cur = parse_node->option->cur->next;
    }

    //arg 그냥 출력. 어차피 따옴표나 환경변수 파싱 단계에서 다 처리 해줘서 그냥 출력만 하면 됨
    parse_node->arg->cur = parse_node->arg->head;
    while (parse_node->arg->cur != 0)
    {
        printf("%s ", parse_node->arg->cur->value);
        parse_node->arg->cur = parse_node->arg->cur->next;
    }
    printf("\b"); //마지막 공백 하나 지워줌
    if (flag_n != 1)
        printf("\n"); //n옵션 있으면 개행 X
}

void config_heredoc(t_lex_list *redirection_list)
{
    redirection_list->cur = redirection_list->head;
    while (redirection_list->cur != NULL)
    {
        if (redirection_list->cur->type == RD_IN_DOUBLE)
            make_heredoc(redirection_list->cur->value);
        redirection_list->cur = redirection_list->cur->next;
    }
}

int config_redirection(t_lex_list *redirection_list)
{
    redirection_list->cur = redirection_list->head;
    while (redirection_list->cur != NULL)
    {
        if (redirection_list->cur->type == RD_IN_SINGLE)
        {
            if (redirection_in(redirection_list->cur->value) == 1)
                return (1);
        }
        else if (redirection_list->cur->type == RD_OUT_SINGLE)
            redirection_out(redirection_list->cur->value);
        else if (redirection_list->cur->type == RD_IN_DOUBLE)
            redirection_heredoc(redirection_list->cur->value);
        else if (redirection_list->cur->type == RD_OUT_DOUBLE)
            redirection_double_out(redirection_list->cur->value);
        redirection_list->cur = redirection_list->cur->next;
    }
    return (0);
}

void connect_pipe(int pipefd[2], int io)
{
    dup2(pipefd[io], io);
    close(pipefd[0]);
    close(pipefd[1]);
}

int is_heredoc(t_lex_list *redirection_list)
{
    redirection_list->cur = redirection_list->head;
    while (redirection_list->cur != NULL)
    {
        if (redirection_list->cur->type == RD_IN_DOUBLE)
            return (1);
        redirection_list->cur = redirection_list->cur->next;
    }
    return (0);
}

void multi_pipe(t_parse_list *parse_list, t_list *envp_list)
{
    int pid;
    int i = 0;
    int status;

    pid = 0;
    parse_list->cur = parse_list->tail;
    while (parse_list->cur != 0 && pid == 0)
    {
        //터미널 설정 원상복구
        tcsetattr(STDIN_FILENO, TCSANOW, &org_term);
        pipe(parse_list->cur->pipefd);
        pid = fork();
        if (pid == 0)
            parse_list->cur = parse_list->cur->prev;
    }
    if (parse_list->cur->index == 0)
    {
        if (parse_list->cur->next == 0)
        {
            config_heredoc(parse_list->cur->redirection);
            if (config_redirection(parse_list->cur->redirection) == 1)
                exit(1);
            if (parse_list->cur->cmd != 0)
            {
                execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
                printf("%s : command not found\n", parse_list->cur->cmd);
                exit(127);
            }
            exit(0);
        }
        else
        {
            config_heredoc(parse_list->cur->redirection);
            connect_pipe(parse_list->cur->next->pipefd, STDOUT_FILENO);
            if (config_redirection(parse_list->cur->redirection) == 1)
                exit(1);
            if (parse_list->cur->cmd != 0)
            {
                execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
                printf("%s : command not found\n", parse_list->cur->cmd);
                exit(127);
            }
            exit(0);
        }
    }
    else if (parse_list->cur->next != 0)
    {
        wait(&status);

        if (is_heredoc(parse_list->cur->redirection) == 0)       //heredoc 없
            connect_pipe(parse_list->cur->pipefd, STDIN_FILENO); //if (rd_list has heredoc)
        else                                                     //heredoc 있
            config_heredoc(parse_list->cur->redirection);
        connect_pipe(parse_list->cur->next->pipefd, STDOUT_FILENO);
        if (config_redirection(parse_list->cur->redirection) == 1)
            exit(1);
        if (parse_list->cur->cmd != 0)
        {
            execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
            printf("%s : command not found\n", parse_list->cur->cmd);
            exit(127);
        }
        exit(0);
    }
    else
    {
        wait(&status);
        if (is_heredoc(parse_list->cur->redirection) == 0)
            connect_pipe(parse_list->cur->pipefd, STDIN_FILENO); //if (rd_list has heredoc)
        else
            config_heredoc(parse_list->cur->redirection);
        if (config_redirection(parse_list->cur->redirection) == 1)
            exit(1);
        if (parse_list->cur->cmd != 0)
        {
            execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
            printf("%s : command not found\n", parse_list->cur->cmd);
            exit(127);
        }
        exit(0);
    }
}

void execute_line(t_parse_list *parse_list, t_list *envp_list, t_list *shell_list)
{
    int length = 0;
    int pid;
    int status;
    int fd_in;
    int fd_out;

    fd_in = dup(STDIN_FILENO);
    fd_out = dup(STDOUT_FILENO);
    parse_list->cur = parse_list->head;
    while (parse_list->cur != 0)
    {
        length++;
        parse_list->cur = parse_list->cur->next;
    }
    parse_list->cur = parse_list->head;//length 대체 parse_list->head->next == NULL
    if (length == 1 && is_builtin(parse_list->cur->cmd) == 0)
    {
        config_heredoc(parse_list->cur->redirection);
        if (config_redirection(parse_list->cur->redirection) == 1)
        {
            free(envp_list->head->value);
            envp_list->head->value = ft_strdup("1");
            dup2(fd_in, STDIN_FILENO);
            dup2(fd_out, STDOUT_FILENO);
            close(fd_in);
            close(fd_out);
            return;
        }
        //if cmd 있음
        //builtin 실행
        if (parse_list->cur->cmd != 0)
        {
            if (ft_strncmp(parse_list->cur->cmd, "echo", 5) == 0)
            {
                ft_echo(parse_list->cur); //parse_node를 보낼까 parse_list를 보낼까 어떤 게 일관성 있을까 고민
                free(envp_list->head->value);
                envp_list->head->value = ft_strdup("0");
            }
            else if (ft_strncmp(parse_list->cur->cmd, "export", 7) == 0)
            {
                ft_export(parse_list->cur, envp_list, shell_list);
            }
            else if (ft_strncmp(parse_list->cur->cmd, "unset", 6) == 0)
            {
                ft_unset(parse_list->cur, envp_list, shell_list);
            }
            else if (ft_strncmp(parse_list->cur->cmd, "env", 4) == 0)
            {
                env(envp_list);
            }
            else if (ft_strncmp(parse_list->cur->cmd, "pwd", 4) == 0)
            {
                pwd(envp_list);
            }
            else if (ft_strncmp(parse_list->cur->cmd, "cd", 3) == 0)
            {
                cd(parse_list->cur->arg->head, envp_list);
            }
            else if (ft_strncmp(parse_list->cur->cmd, "exit", 5) == 0)
            {
                ft_exit(parse_list->cur);
            }
        }
        dup2(fd_in, STDIN_FILENO);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_in);
        close(fd_out);
        //메인에서 실행, return으로 끝
    }
    else
    {
        pid = fork();
        if (pid == 0)
            multi_pipe(parse_list, envp_list);
        else
            wait(&status); //
        free(envp_list->head->value);
        envp_list->head->value = ft_itoa(status / 256);
        //터미널 세팅 미니쉘버전 변경
        tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    }
}

//
//부모 $?=0
//제일 먼저 실행 zesf 실행되기 전 $?= 0 실행되면 $? = 127
//|
//제일 먼저 실행 2 echo $? 실행되기 전 $?= 0 실행되면 $? = 0
//
//ls 정상
// zzzzz | echo $?

//