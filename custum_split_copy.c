#include "libft/libft.h"

char	**arr_free(char **arr)
{
	unsigned int i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

int skip_quotes(char const *s, char q)
{
	int i = 0;
	if (s[i] == q)
		i++;
	while(s[i] != q && s[i] != '\0')
		i++;
	if (s[i] == q)
		i++;
	return (i);
}

static int cnt_word(char const *s, char c)
{
	int i = 0;
	int cnt = 0;

	if (s == 0)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] == '"')
			i += skip_quotes(&s[i], '"');
		if (s[i] == '\'')
			i += skip_quotes(&s[i], '\'');
		if (s[i] == '\0')
			break;
		if (s[i] == c || s[i + 1] == '\0')//"aa"[\0]
		{
			while (s[i] == c && s[i] != '\0')
				i++;
			cnt++;
		}
		i++;
	}
	return (cnt);
}

static int cnt_letter(char const *s, char c)
{
	int i = 0;

	if (s == 0)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] == '"')
			i += skip_quotes(&s[i], '"');
		if (s[i] == '\'')
			i += skip_quotes(&s[i], '\'');
		if (s[i] == '\0')
			return (i);
		if (s[i] == c || s[i + 1] == '\0')
			return (i);
		i++;
	}
	return (0);
}

static char **arr_fill(char **arr, char const *s, char c)
{
	int i = 0;
	int j = 0;
	
	arr = (char **)malloc(sizeof(char *) * (cnt_word(s, c) + 1));//
	if (arr == 0 || s == 0)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] != c && s[i] != '\0')
		{
			arr[j] = ft_substr(&s[i], 0, cnt_letter(&s[i], c));//quote_flag 사용
			i += cnt_letter(&s[i], c);
			if (arr[j] == 0)
				return (0);
			j++;

			if (s[i] == '\0')
				break;
		}
		i++;
	}
	arr[j] = 0;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char **arr;

	if (s == NULL)
		return (0);
	arr = NULL;
	printf("101 : %s\n", s);
	if ((arr = arr_fill(arr, s, c)) == NULL)
		return (NULL);
	return (arr);
}

int main(void)
{
	char **arr;
	int i = 0;

	arr = ft_split("    a\"  a$AA  \"  b c'd e'f   ggg \"aa\" a     ", ' ');
	while (arr[i] != 0)
	{
		printf("[%d] : [%s]\n", i, arr[i]);
		i++;
	}
	//arr_free(arr);
    return (0);
}