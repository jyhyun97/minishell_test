#include "minishell.h"

    //필요 함수
    //1. cmd가 빌트인인지 확인하는 함수 int 0 1 완성!
    //2. cmd path 찾아주는 함수 char * 완성!
    //3. 옵션, 인자 2차원 배열로 만드는 함수 char ** 완성!
    
int count_lex_node(t_lex_list *lex_list)
{
    int num;

    num = 0;
    lex_list->cur = lex_list->head;
    while (lex_list->cur != 0)
    {
        lex_list->cur = lex_list->cur->next;
        num++;
    }
    return (num);
}

char **make_argv(t_parse_node *parse_node, t_list *envp_list)
{
    char **new_argv;
    int cnt_all_node;
    int i;

    cnt_all_node = count_lex_node(parse_node->option) + count_lex_node(parse_node->arg);
    new_argv = (char **)malloc(sizeof(char *) * (cnt_all_node + 2));
    i = 0;
    parse_node->option->cur = parse_node->option->head;
    new_argv[i] = parse_node->cmd; //cmd , minishell 파일이름..
    i++;
    while (parse_node->option->cur != 0)
    {
        new_argv[i] = parse_node->option->cur->value;
        parse_node->option->cur = parse_node->option->cur->next;
        i++;
    }
    parse_node->arg->cur = parse_node->arg->head;
    while (parse_node->arg->cur != 0)
    {
        new_argv[i] = parse_node->arg->cur->value;
        parse_node->arg->cur = parse_node->arg->cur->next;
        i++;
    }
    new_argv[i] = 0;

    //test 나중에 필히 삭제!!!!
    i = 0;
    while (new_argv[i] != 0)
    {
        printf("new_argv : %s\n", new_argv[i]);
        i++;
    }

    return (new_argv);
}

int is_builtin(char *cmd)
{
    if (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "echo", 4) == 0 ||
        ft_strncmp(cmd, "export", 6) == 0 || ft_strncmp(cmd, "unset", 5) == 0 ||
        ft_strncmp(cmd, "env", 3) == 0 || ft_strncmp(cmd, "pwd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 3) == 0)
        return (0);//빌트인
    else
        return (1);//빌트인 아님
}

char *make_path(char *cmd, t_list *envp_list)
{
    char **bins;
    char *new_path;
    char *tmp;
    int i;

    envp_list->cur = envp_list->head;
    while (envp_list->cur != 0)
    {
        if (ft_strncmp(envp_list->cur->key, "PATH", 4) == 0)
            break;
        envp_list->cur = envp_list->cur->next;
    }
    if (envp_list->cur == 0)
        return (0);
    bins = ft_split(envp_list->cur->value, ':');
    i = 0;
    while (bins[i] != 0)
    {
        tmp = ft_strjoin(bins[i], "/");
        new_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(new_path, R_OK) != -1)
        {
            arr_free(bins);
            return (new_path);
        }
        i++;
    }
    arr_free(bins);
    return (0);
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
    //원한는 index 노드에 점근할수 있어야함.
    //마지막 부터 fork해주어야 함.
    //
    parse_list->cur = parse_list->tail;
    while (parse_list->cur != 0 && pid == 0) // 자식 프로세스 pid == 0
    {
        pipe(parse_list->cur->pipefd); //fd == 0 출구 fd == 1 입구,
        pid = fork();
        wait(&status);
        if (pid == 0)
            parse_list->cur = parse_list->cur->prev;
    }
        printf("pid : %d  index: %d \n", pid, parse_list->cur->index);
    exit(0);

    // execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);

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
        //execve(make_path(parse_list->cur->cmd, envp_list), make_argv(parse_list->cur, envp_list), 0);

    }
    else
    {
        printf("many\n");
        multi_pipe(parse_list, envp_list);
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
        printf("[%d] : [%s]\n", i, tokens[i]);
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
        printf("%s, %d\n", parse_list->cur->cmd, parse_list->cur->index);
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
        rl_redisplay();
        //rl_replace_line("\n", 0);
    }
    //reset_input_mode();

    return (0);
}