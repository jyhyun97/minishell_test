#include "minishell.h"

int count_tokens(char **tokens)
{
    int cnt = 0;
    int i = 0;
    int j = 0;
    while (tokens[i] != 0)
    {
        j = 0;
        while (tokens[i][j] != '\0')
        {
            if (ft_strncmp(&tokens[i][j], "<<", 2) == 0 || ft_strncmp(&tokens[i][j], ">>", 2) == 0)
                j += 2;
            else if (tokens[i][j] == '<' || tokens[i][j] == '>' || tokens[i][j] == '|')
                j++;
            else
            {
                while (tokens[i][j] != '\0' && tokens[i][j] != '<' && tokens[i][j] != '>' && tokens[i][j] != '|')
                {
                    if (tokens[i][j] == '\'')
                        j += skip_quotes(&tokens[i][j], '\'');
                    else if (tokens[i][j] == '"')
                        j += skip_quotes(&tokens[i][j], '"');
                    else
                        j++;
                }
            }
            cnt++;
        }
        i++;
    }

    return (cnt);
}

char **divide_tokens(char **tokens)
{
    char **new_arr;
    int i = 0;
    int j = 0;
    int k = 0;
    int start = 0;

    new_arr = (char **)malloc(sizeof(char *) * (count_tokens(tokens) + 1));
    while (tokens[i] != 0)
    {
        j = 0;
        while (tokens[i][j] != '\0')
        {
            start = j;//80~95행 나중에 함수화 해서 빼기 (count tokens, divide tokens 사용)
            if (ft_strncmp(&tokens[i][j], "<<", 2) == 0 || ft_strncmp(&tokens[i][j], ">>", 2) == 0)
                j += 2;
            else if (tokens[i][j] == '<' || tokens[i][j] == '>' || tokens[i][j] == '|')
                j++;
            else
            {
                while (tokens[i][j] != '\0' && tokens[i][j] != '<' && tokens[i][j] != '>' && tokens[i][j] != '|')
                {
                    if (tokens[i][j] == '\'')
                        j += skip_quotes(&tokens[i][j], '\'');
                    else if (tokens[i][j] == '"')
                        j += skip_quotes(&tokens[i][j], '"');
                    else
                        j++;
                }
            }
            new_arr[k] = ft_substr(tokens[i], start, j - start);
            k++;
        }
        i++;
    }
    new_arr[k] =0;
    //free(tokens);
    arr_free(tokens);
        
    return (new_arr);
}
