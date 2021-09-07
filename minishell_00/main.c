#include "minishell.h"

int		redirection_in(char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int		redirection_out(char *file)
{
	int fd;

	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int		redirection_double_out(char *file)
{
	int fd;

	fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

void    make_heredoc(char *delimiter)
{
    char *line;
    int fd;


	fd = open("heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    while (1)
    {
        line = readline("> ");
        if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
            break;
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
}

int     redirection_heredoc(char *delimiter)
{
    //char *line;
    // int fd;


	// fd = open("heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    // while (1)
    // {
    //     line = readline("> ");
    //     if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
    //         break;
    //     write(fd, line, ft_strlen(line));
    //     write(fd, "\n", 1);
    //     free(line);
    // }
    // close(fd);
    redirection_in("heredoc_tmp");
    unlink("heredoc_tmp");
    return (0);
}

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

    //index로 처음중간 끝 구분
    
    // if (pid == 0)
        // exit(0);
}

void    execute_line(t_parse_list *parse_list, t_list *envp_list)
{
    /*
        전처리 에서 구현 해야할 부분.
        execve 인자로 넘겨줄때 리턴값으로 줘도될듯.
            옵션, 인자 2차원 배열로 만들어 주기.
            cmd 경로 찾아서 1개의 문자열로 만들어 주기. 
        
        파이프 유무 판단.
        
        pipe;
        빌트인은 따로처리. //자식프로세스에서 끝낼때 exit로  종료, 부모프로세스에서는 return 으로 다르게 처리.
        execve(명령어, 옵션(2차원 배열), 환경변수);
        명령어 경로 찾기
        옵션값 2차원 배열로 만들어 주기
        // 환경변수는? ......;; 2차원으로 만들어 주어야 하나.. env 빌트인 만들떄 2차원으로 변환

        //int status;
        //wait(&status);
        //종료 상태값
        명령어 1개 실행 되더라도 자식 프로세스에서 실행되어야 한다.
        //execve는 자식 프로세스, 빌트인은 메인 프로세스에서
        //자식 프로세스에서 빌트인 들어올 경우 리턴되는 status값 exit (0)(true) (1)(false)
        //
            all->exit_status = 0;
            all->exit_status = 1;
        
    */


    int i = 0; 
    int length = 0;
    int pid;
    int status;
    //cd ../../minishell
    //chdir("../../minishell");
    //PWD = getcwd(매개변수)
    //getcwd
    //파이프 개수 확인.
    parse_list->cur = parse_list->head;
    while (parse_list->cur != 0)
    {
        length++;
        parse_list->cur = parse_list->cur->next;
    }
    parse_list->cur = parse_list->head;
    if (length == 1 && is_builtin(parse_list->cur->cmd) == 0)
    {
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
        
        // execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);
        //자식프로세스 함수(테스트 프로그램 만들어 둔 것 응용)
        // 자식프로세스에서 실행(파이프 연결)
        //     if(빌트인)
        //         결과만 return
        //     else (execve)
        //         execve 끝
    }
}


t_parse_list *parse_line(char *line, t_list *envp_list)
{
    char **tokens;
    int i = 0;
    t_lex_list *lex_list;
    t_parse_list *parse_list;
    
    //tokenizing
    if (line == 0)
        return (0);
    line = ft_strtrim(line, " ");
    tokens = word_split(line, ' ');//"> " ">"
    tokens = convert_env(tokens, envp_list);
    tokens = divide_tokens(tokens);
    tokens = trim_tokens(tokens); //순서 바꿈. "ls|env"와 ls|env의 구별이 불가능해서
    while (tokens[i] != 0)
    {
        // printf("[%d] : [%s]\n", i, tokens[i]);
        i++;
    }
    //lexicalizing
    init_lex_list(&lex_list);
    Lexicalize_token(tokens, lex_list);
    
    //parsing
    init_parse_list(&parse_list);
    parse_lexer(parse_list, lex_list);

    parse_list->cur = parse_list->head;
    while (parse_list->cur != 0)
    {
        // printf("%s, %d\n", parse_list->cur->cmd, parse_list->cur->index);
        parse_list->cur = parse_list->cur->next;
    }//qwe asd zxc qwe 여러번 반복 시 segfalut 발생
    return (parse_list);
    //arr_free(tokens);
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
    t_parse_list *parse_list;


    //save_input_mode();
    //set_input_mode();
    //signal_initialize();

    envp_list_initialize(envp, &envp_list);

    while (1)
    {
        line = readline("minishell$ "); //공뱍들어왔을떄 처리
        if (line && *line)
            add_history(line);
        parse_list = parse_line(line, envp_list);
        //실행부
        execute_line(parse_list, envp_list);
        //프리
        free(line);
        // rl_redisplay();
        // rl_replace_line("\n", 0);
    }
    //reset_input_mode();

    return (0);
}
