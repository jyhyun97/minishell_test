#include <termios.h>
#include <stdio.h>
#include <unistd.h>

/*
    int tcgetattr(int fd, struct termios *termios_p);

    int tcsetattr(int fildes, int optional_actions, struct termios *termios_p);

*/

struct termios org_term;
struct termios new_term;

void save_input_mode(void)
{
    tcgetattr(STDIN_FILENO, &org_term); // STDIN으로부터 터미널 속성을 받아온다
}

void set_input_mode(void)
{
    tcgetattr(STDIN_FILENO, &new_term);          // STDIN으로부터 터미널 속성을 받아온다
    new_term.c_lflag &= ~(ICANON | ECHO);        // ICANON, ECHO 속성을 off
    new_term.c_cc[VMIN] = 1;                     // 1 바이트씩 처리
    new_term.c_cc[VTIME] = 0;                    // 시간은 설정하지 않음
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term); // 변경된 속성의 터미널을 STDIN에 바로 적용
}

// 기존의 터미널 세팅으로 다시 변경
void reset_input_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &org_term); // STDIN에 기존의 터미널 속성을 바로 적용
}

int main(void)
{
	int ch = 0;
	int idx = -1;

	save_input_mode();
	set_input_mode();
	while (read(0, &ch, 1) > 0)
	{
		if (ch == 4)
		{
            write(0, "check\n",6);
			if (idx == -1)
				return 0;
			else
				continue;
		}
		else if (ch == 127)
		{
			if (idx >= 0)
			{
				--idx;
				write(0, "\b \b", 3);
			}
		}
		else if (ch == '\n')
			break;
		else
		{
			++idx;
			write(0, &ch, sizeof(int));
		}
		ch = 0;
	}
	reset_input_mode();
	return (0);
}