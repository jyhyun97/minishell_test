#include "minishell.h"
#include "list.h"

void parse_line(char *line, t_list *envp_list)
{
    char *tmp;
    char *tmp2;
    int i;

    printf("line : %s\n", line);
    i = 0;
    while(line[i] != '\0')
    {
        if (line[i] == '\"')
        {
            i++;
            // char *token;
            while (line[i] == '\"')
            {

            }
                // token = ft_substr(line[시작점], 끝점);
                // tmp2 = ft_strjoin(tmp, token);
                // free(token)
        }
        else if (line[i] == '\'')
        {
            //다음 '까지의 값을 저장
        }
        else
        {
            //그냥 저장
        }
        i++;
    }
    printf("tmp : %s\n", tmp);

    //tmp = 123456
    //"aaa'a'" |"bbb" |"ccc"cc"" | "$tmp"
    
    //1. "aaa'a'" |"bbb" |"ccc"cc"" | "$tmp"
    //2. aaa'a' |bbb |ccccc | $tmp
    //3. aaa'a' aaa |bbb |ccccc | 123456
    //4. 구조체 저장 [명령어, aaa'a'], [옵션, zzz], [옵션, xxx], [인자, aaa], [파이프, |], [명령어, ccccc], [파이프, |], [명령어, 123456]
    //5. 프로세스 구조체 저장 [명령어, 옵션(하나의 문자열), 인자, 파이프], [명령어, 파이프], [명령어]

    //따옴표 : 첫번째 등장 ~ 두번째 등장까지 왼->오 방향
    //"a'a'a"a"a'a'a"


    //aaaa abds"asdb" adbab "ccc"
    //단순 문자열 분리
    //
    //         "aaaa$A abdsas"db"| 'adbdb' | ccc"
    //char *~~ "aaaa1234 abdsasdb| adbab | ccc" 한 줄 만들기
    //line에서 "" '' 분리
    //화이트 스페이스 분리
    //파이프, 리다이렉션
    //환경변수 대입

    //" "as" "   "as"
    //
    //
    //분리된 정보 token_list저장

    //token_list를 process_list로
}


int main(int argc, char **argv, char **envp)
{
    char *line;
    t_list *envp_list;

	//save_input_mode();
	//set_input_mode();
    //signal_initialize();
    
    envp_list_initialize(envp, &envp_list);

    while (1)
    {
        line = readline("minishell$ ");//
        if (line && *line)
            add_history(line);
        parse_line(line, envp_list);
        //
        free(line);
        rl_redisplay();
        rl_replace_line("\n", 0);
    }
  	//reset_input_mode();

    return (0);
}

//  export LDFLAGS="-L/opt/homebrew/opt/readline/lib"
//  export CPPFLAGS="-I/opt/homebrew/opt/readline/include"
