#include "minishell.h"


void config_heredoc(t_lex_list *redirection_list)
{
    redirection_list->cur = redirection_list->head;
    while(redirection_list->cur != NULL)
    {
        if (redirection_list->cur->type == RD_IN_DOUBLE)
            make_heredoc(redirection_list->cur->value);
        redirection_list->cur = redirection_list->cur->next;
    }
}

void    config_redirection(t_lex_list *redirection_list)
{
    redirection_list->cur = redirection_list->head;
    while(redirection_list->cur != NULL)
    {
        if (redirection_list->cur->type == RD_IN_SINGLE)
            redirection_in(redirection_list->cur->value);
        else if (redirection_list->cur->type == RD_OUT_SINGLE)
            redirection_out(redirection_list->cur->value);
        else if (redirection_list->cur->type == RD_IN_DOUBLE)
            redirection_heredoc(redirection_list->cur->value);
        else if (redirection_list->cur->type == RD_OUT_DOUBLE)
            redirection_double_out(redirection_list->cur->value);
        redirection_list->cur = redirection_list->cur->next;
    }
}

void    connect_pipe(int pipefd[2], int io)
{
    dup2(pipefd[io], io);
    close(pipefd[0]);
    close(pipefd[1]);
}

int is_heredoc(t_lex_list *redirection_list)
{
    int rtn;

    rtn = 0;
    redirection_list->cur = redirection_list->head;
    while(redirection_list->cur != NULL)
    {
        if (redirection_list->cur->type == RD_IN_DOUBLE)
        {
             rtn = 1;
             return rtn;
        }
        redirection_list->cur = redirection_list->cur->next;
    }
    return rtn;
}

void multi_pipe(t_parse_list *parse_list, t_list *envp_list)
{
    int length;
    int pid;
    int i = 0;
    int status;

    length = 0;
    pid = 0;
    parse_list->cur = parse_list->head;
    while (parse_list->cur != 0)
    {
        length++;
        parse_list->cur = parse_list->cur->next;
    }
    parse_list->cur = parse_list->tail;
    while (parse_list->cur != 0 && pid == 0)
    {
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
            config_redirection(parse_list->cur->redirection);
            execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
        }
        else
        {
            config_heredoc(parse_list->cur->redirection);
            connect_pipe(parse_list->cur->next->pipefd, STDOUT_FILENO);
            config_redirection(parse_list->cur->redirection);
            execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);        
        }
    }
    else if (parse_list->cur->next != 0)
    {
        wait(&status);

        if (is_heredoc(parse_list->cur->redirection) == 0)//heredoc 없
            connect_pipe(parse_list->cur->pipefd, STDIN_FILENO);//if (rd_list has heredoc)
        else//heredoc 있
            config_heredoc(parse_list->cur->redirection);
        connect_pipe(parse_list->cur->next->pipefd, STDOUT_FILENO);
        config_redirection(parse_list->cur->redirection);
        execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
    }
    else
    {
        wait(&status);
        if (is_heredoc(parse_list->cur->redirection) == 0)
            connect_pipe(parse_list->cur->pipefd, STDIN_FILENO);//if (rd_list has heredoc)
        else
            config_heredoc(parse_list->cur->redirection);
        config_redirection(parse_list->cur->redirection);
        execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
    }
}

void    execute_line(t_parse_list *parse_list, t_list *envp_list)
{
    int i = 0; 
    int length = 0;
    int pid;
    int status;

    parse_list->cur = parse_list->head;
    while (parse_list->cur != 0)
    {
        length++;
        parse_list->cur = parse_list->cur->next;
    }
    parse_list->cur = parse_list->head;
    //

    if (length == 1 && is_builtin(parse_list->cur->cmd) == 0)
    {
        //config_redirection(parse_list->cur->redirection); //에러
        //redirection 함수
        //if cmd 있음
            //builtin 실행
        //메인에서 실행, return으로 끝
        printf("length 1 and builtin true\n");
    }
    else
    {
        pid = fork();
        // printf("many\n");
        if (pid == 0)
            multi_pipe(parse_list, envp_list);
        else
            wait(&status);
    }
}
