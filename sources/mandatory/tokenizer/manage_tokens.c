#include "../../../includes/mandatory/mini_shell.h"

static void	expand_status_venv(char *env_key, char **final_line, \
									t_data *data)
{
	char	*temp;
	char	*status;

	status = ft_itoa(data->for_sts);
	temp = ft_strjoin(*final_line, status);
	free (*final_line);
	free (status);
	free (env_key);
	*final_line = temp;
	return ;
}

void	env_entry(char *env_key, char **final_line, t_venv **envp)
{
	t_venv	*env;
	char		*temp;

	temp = NULL;
	env = env_lstsearch(envp, env_key);
	free (env_key);
	if (env != NULL)
	{
		temp = ft_strjoin(*final_line, env->value);
		free (*final_line);
		*final_line = temp;
	}
	else
	{
		if (*final_line)
			free (*final_line);
		*final_line = NULL;
	}
}

char	*adjust_venv(int *i, char *content, char **final_line, t_data *data)
{
	int			start;
	char		*env_key;

	start = *i;
	(*i)++;
	data->has_env = TRUE;
	while (content[*i])
	{
		if (ft_isspace(content[*i]) == 0 && content[*i] != '$' && \
			content[*i] != '\'' && content[*i] != '\"')
		{
			if (content[++(*i) - 1] == '?')
				break ;
		}
		else
			break ;
	}
	env_key = ft_strndup(&content[start + 1], (*i) - start - 1);
	if (env_key[0] == '?')
	{
		expand_status_venv(env_key, final_line, data);
		return (NULL);
	}
	return (env_key);
}

void	for_words(int *i, char *content, char **final_line, int has_s_quote)
{
	char	*line;
	char	*temp;
	int		start;

	start = *i;
	while (content[*i])
	{
		if (content[*i] == '$' && has_s_quote == FALSE && \
			content[(*i) + 1] && ft_isspace(content[(*i) + 1]) == 0 \
			&& content[(*i) + 1] != '\'' && content[(*i) + 1] != '\"')
			break ;
		(*i)++;
	}
	line = ft_strndup(&(content[start]), (*i) - start);
	temp = ft_strjoin(*final_line, line);
	free (*final_line);
	free (line);
	*final_line = temp;
}

t_token	*fixed_etokens(t_token *token, char **final_line, t_data *data)
{
	t_token	*next;

	free (token->content);
	if (*final_line != NULL)
	{
		token->content = *final_line;
		return (token);
	}
	else
	{
		next = token->next;
		if (token->prev)
			token->prev->next = token->next;
		if (token->next)
			token->next->prev = token->prev;
		if (next)
			data->token_list = next;
		else
			data->token_list = token->prev;
		free (token);
		return (next);
	}
}
