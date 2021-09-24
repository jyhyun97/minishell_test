#include "minishell.h"


void sig_int(int sig_number)
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG);

    if (sig_number == SIGINT)
    {
        if (pid == -1)
        {
            write(1, "\n", 1);
            rl_replace_line("", 0);
            rl_on_new_line();
            rl_redisplay();
        }
        else
        {
            write(1,"\n", 1);
        }
    }
    else if (sig_number == SIGQUIT)
    {
        if (pid != -1)
        {
            write(1, "Quit: 3\n", 8);
        }
    }
    
}

void signal_initialize(void)
{
    signal(SIGINT, sig_int);
    signal(SIGQUIT, sig_int);

}


void save_input_mode(void)
{
    tcgetattr(STDIN_FILENO, &org_term); // STDIN으로부터 터미널 속성을 받아온다
}

void set_input_mode(void)
{
    tcgetattr(STDIN_FILENO, &new_term);          // STDIN으로부터 터미널 속성을 받아온다
    new_term.c_lflag &= ECHO;//~(ICANON /*| ECHO*/);        // ICANON, ECHO 속성을 off
    new_term.c_cc[VMIN] = 1;                     // 1 바이트씩 처리
    new_term.c_cc[VTIME] = 0;                    // 시간은 설정하지 않음
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term); // 변경된 속성의 터미널을 STDIN에 바로 적용
}

// 기존의 터미널 세팅으로 다시 변경
void reset_input_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &org_term); // STDIN에 기존의 터미널 속성을 바로 적용
}