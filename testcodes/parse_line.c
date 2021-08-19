
void init_zero(char *arr)
{
	int	i = 0;
	while (i < 100)
	{
		arr[i] = '\0';
		i++;
	}
}

void parse_line(char *line)
{
	printf("line : %s\n", line);

	int i = 0;
	int j = 0;
	char tmp[100];
	
	init_zero(tmp);
	i = 0;
	while (line[i] != '\0')//"aa"bb
	{
		if (line[i] == '"' && line[i + 1] != '\0')
		{
			i++;
			while(line[i] != '"')//line : "aa"bb tmp :  aa
			{
				if (line[i] == '$')
				{
					char envp_key[100];
					int a = 0;
					char *envp_value;
					init_zero(envp_key);
					i++;
					while (line[i] != '"' && line[i] != ' ' && line[i] != '\t' && line[i] != '\0')
					{
						envp_key[a] = line[i];
						a++;
						i++;
					}
					envp_key[a] = '\0';
					envp_value = getenv(envp_key); //free해줘야 할까..
					if (envp_value == 0)
						break;
					a = 0;
					while (envp_value[a] != '\0')
					{
						tmp[j] = envp_value[a];
						a++;
						j++;
					}
				}
				else
				{
					tmp[j] = line[i];
					j++;
					i++;
				}
			}
			i++;
		}
		else if(line[i] == '\'' && line[i + 1] != '\0')
		{
			i++;
			while(line[i] != '\'')
			{
				tmp[j] = line[i];
				j++;
				i++;
			}
			i++;
		}
		else if (line[i] == '$')
		{
			char envp_key[100];
			int a = 0;
			char *envp_value;
			init_zero(envp_key);
			i++;
			while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0')
			{
				envp_key[a] = line[i];
				a++;
				i++;
			}
			envp_key[a] = '\0';
			envp_value = getenv(envp_key); //free해줘야 할까..
			if (envp_value == 0)
				break;
			a = 0;
			while (envp_value[a] != '\0')
			{
				tmp[j] = envp_value[a];
				a++;
				j++;
			}
		}
		else
		{
			tmp[j] = line[i];
			j++;
			i++;
		}
	}
	tmp[j] = '\0';
	printf("tmp : %s\n", tmp);
	//왜 split을 활용하는 지 알곘다
}