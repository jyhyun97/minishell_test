#include "minishell.h"

int count_trimrd_token(char *token)
{
    int i = 0;
    
    while (token[i] != '\0')
    {
        if (token[i] == '"')
        {
            i++;
            while (token[i] != '"')
                i++;
            i++;
        }
        else if (token[i] == '\'')
        {
            i++;
            while (token[i] != '\'')
                i++;
            i++;
        }
        else
            i++;
    }
    return (i);
}

char *trim_quote(char *token)
{
    /*
        따옴표만 제거하기.
        따옴표 제외한 글자세기.
    */
    char *new_str;
    int i = 0;
    int j = 0;
    new_str = (char *)malloc(sizeof(char) * (count_trimrd_token(token) + 1));
    
    while (token[i] != '\0')
    {
        if (token[i] == '"')
        {
            i++;
            while (token[i] != '"')
            {
                new_str[j] = token[i];
                i++;
                j++;    
            }
            i++;
        }
        else if (token[i] == '\'')
        {
            i++;
            while (token[i] != '\'')
            {
                new_str[j] = token[i];
                i++;
                j++;
            }
            i++;
        }
        else
        {
            new_str[j] = token[i];
            i++;
            j++;
        }
    }
    new_str[j] = '\0';
    free(token);
    return (new_str);
}

char **trim_tokens(char **tokens)
{
    /*
        전체 tokens의 내용의 따옴표 제거
    */
    int i = 0;
    while(tokens[i] != NULL)//
    {
        tokens[i] = trim_quote(tokens[i]);
        i++;
    }
    return (tokens);
}