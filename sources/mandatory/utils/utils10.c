#include "../../../includes/mandatory/mini_shell.h"

static int	get_newargs(t_token *current, int direction)
{
	if (direction == TO_RIGHT)
	{
		while (current->next)
		{
			if (current->next->token == WORD)
				current->next->token = CMD_TOKEN;
			else
				break ;
			current = current->next;
		}
	}
	else
	{
		while (current->prev)
		{
			if (current->prev->token == WORD)
				current->prev->token = CMD_TOKEN;
			else
				break ;
			current = current->prev;
		}
	}
	return (TRUE);
}

static int	get_newopr(t_token *current)
{
	char	*new_content;

	if ((current->token >= 3 && current->token <= 5) && current->next)
	{
		if (current->next->token == WORD)
		{
			current->next->token = FILE_TOKEN;
			new_content = remove_quotes_expansion(current->next->content);
			free(current->next->content);
			current->next->content = new_content;
		}
	}
	else if (current->token == FILE_TOKEN && current->next)
	{
		if (current->next->token == WORD)
			get_newargs(current, TO_RIGHT);
	}
	if ((current->token >= 3 && current->token <= 5) && current->prev)
	{
		if (current->prev->token == WORD)
			get_newargs(current, TO_LEFT);
	}
	return (TRUE);
}

static int	for_nextpipe(t_token *current)
{
	if (current->token == PIPE && current->next)
	{
		if (current->next->token == WORD)
			get_newargs(current, TO_RIGHT);
	}
	if (current->token == PIPE && current->prev)
	{
		if (current->prev->token == WORD)
			get_newargs(current, TO_LEFT);
	}
	return (TRUE);
}

static int	for_dless(t_token *current)
{
	if (current->token == DLESS && current->next)
	{
		if (current->next->token == WORD)
			current->next->token = DELIMITER_TOKEN;
	}
	else if (current->token == DELIMITER_TOKEN && current->next)
	{
		if (current->next->token == WORD)
			get_newargs(current, TO_RIGHT);
	}
	if (current->token == DLESS && current->prev)
	{
		if (current->prev->token == WORD)
			get_newargs(current, TO_LEFT);
	}
	return (TRUE);
}

t_token	*organize_eotokens(t_token *current, int *has_operator,
				t_data *data, t_venv **envp)
{
	if (current->next == NULL)
	{
		current->token = CMD_TOKEN;
		current = get_envar_fornext(current, envp, data);
		return (reorganize_tokens(&(data->token_list)));
	}
	while (current->next)
	{
		if ((current->token >= 3 && current->token <= 5) && current->next)
			*has_operator = get_newopr(current);
		else if ((current->token == DLESS || current->token == DELIMITER_TOKEN)
			&& current->next)
			*has_operator = for_dless(current);
		else if (current->token == PIPE && current->next)
			*has_operator = for_nextpipe(current);
		if (((current->token >= 3 && current->token <= 5)
				|| current->token == FILE_TOKEN) && current->prev)
			*has_operator = get_newopr(current);
		else if (current->token == DLESS && current->prev)
			*has_operator = for_dless(current);
		else if (current->token == PIPE && current->prev)
			*has_operator = for_nextpipe(current);
		current = current->next;
	}
	return (current);
}