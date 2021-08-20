#include "minishell.h"



int measure_env_key(char *str)
{              
      /*
        |
        >
        <
        "
        '
        $
        ' '    
    */
    int i = 0;//abc 
    while (str[i] != '\0')
    {
        if (str[i] == ' ' || str[i] == '$' || str[i] == '"' || str[i] == '\'' ||
            str[i] == '|' || str[i] == '>' || str[i] == '<' )
        {
            break;
        }
        i++;
    }
    return (i);
}


char *new_arr_str(char *arr_str, char *envp_key, t_list *envp_list, int length)
{
    /*
                char *new_arr_str(arr[n], tmp, measure env key)
                {
                    char *new_str;
                    i = 0
                    j = 0
                    k = 0
                    new_str = malloc(strlen(arr[i]) + get_env(tmp, envp_list) + 1)
                    while (arr[i] != '\0')
                    {
                        if (arr[i] == $)
                        char *AAAA = get_env(tmp, envp_list);
                            new_str[j] = AAAA[k];
                            k++ j++
                            //i도 measure env key 만큼 증가 시켜주기
                            free AAAA
                        else
                            new_str[j] = arr[i];
                            i++ j++
                    }
                    new_str[j] = '\0';
                    //free
                    return (new_str);
                }
                */
    char *new_str;
    char *envp_value = get_env(envp_key, envp_list);
    int i = 0;
    int j = 0;
    new_str = malloc(ft_strlen(arr_str) + ft_strlen(envp_value) + 1);
    while (arr_str[i] != '\0')
    {
        if (arr_str[i] == '$')
        {
            while (envp_value[j] != '\0')
            {
                new_str[i + j]
                j++;
            }
        }
        else
        {
            new_str[i] = arr_str[i];
        }
        i++;
    }

}

char *get_env(char *key, t_list *envp_list)
{
    int i = 0;
    //리스트 탐색.
    envp_list->cur = envp_list->head;
    while(envp_list->cur != NULL)
    {
        if (ft_strncmp(envp_list->cur->key, key, ft_strlen(key)) == 0) // 문자 비교
            return (envp_list->cur->value);
        envp_list->cur = envp_list->cur->next;
    }
    return (NULL);
}


char **convert_env(char **arr, t_list *envp_list)
{
    //$있는지?
    //""는 있는지?  "" 안에 $가 있는지.
    //$이후 문자 받아는 기능    

    int i = 0;
    int j = 0;
    char *tmp;
    while (arr[i] != 0)
    {
        j = 0;
        while(arr[i][j] != '\0')
        {
            if (arr[i][j] == '$')//
            {
                j++;
                
                //$이후 문자 가져오기
                tmp = ft_substr(&arr[i][j], 0, measure_env_key(&arr[i][j]));
                
                free(arr[i]);
                arr[i] = get_env(tmp, envp_list);
                free(tmp);
                //printf("63 : %s\n", tmp);
                //printf("%s\n", get_env(tmp, envp_list));
                //환경변수에서 검색해서 값 치환.
            }
            if (arr[i][j] == '"')
            {
                //$가 있는지 확인
                //$가 있으면 값치환.
            }
            if (arr[i][j] == '\'')
            {
                //그냥 넘어가기
            }
            j++;
        }
        i++;
    }
    return (arr);
}



void parse_line(char *line, t_list *envp_list)
{
    char **arr;
    int i = 0;
    arr = word_split(line, ' ');

    //환경볁수 처리

    convert_env(arr, envp_list);
    while (arr[i] != 0)
	{
		printf("[%d] : [%s]\n", i, arr[i]);
		i++;
	}
    arr_free(arr);
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

	//save_input_mode();
	//set_input_mode();
    //signal_initialize();
    
    envp_list_initialize(envp, &envp_list);

    while (1)
    {
        line = readline("minishell$ ");//
        
        if (line && *line)
            add_history(line);    
        //파서 ft_sprit();
        parse_line(line, envp_list);
        
        free(line);
        rl_redisplay();
        rl_replace_line("\n", 0);
    }
  	//reset_input_mode();

    return (0);
}

//  export LDFLAGS="-L/opt/homebrew/opt/readline/lib"
//  export CPPFLAGS="-I/opt/homebrew/opt/readline/include"