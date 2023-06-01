/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 17:48:06 by ooutabac          #+#    #+#             */
/*   Updated: 2023/05/27 18:32:29 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* LEXER
How it works:
1)
*/
t_shell_s	*lexer(t_shell_s *minishell, char *str)
{
	t_counter	count;

	if (!minishell)
		return (NULL);
	if (!str)
		return (minishell);
	count.i = 0;
	count.j = 0;
	if (number_of_dquotes(str) % 2 == 1 || number_of_squotes(str) % 2 == 1)
	{
		free_everything(minishell);
		printf("Minishell: Error: lexer: Odd number of quotes\n");
		return (NULL);
	}
	minishell->lexer = ft_calloc(sizeof(t_lexer), 1);
	minishell->lexer->num_of_tokens = num_of_tokens(str);
	minishell->lexer->tokens = ft_calloc(sizeof(char *), (minishell->lexer->num_of_tokens + 1));
	minishell->lexer->tokens[0] = NULL;
	while (str[count.i] && count.j < minishell->lexer->num_of_tokens)
	{
		minishell->lexer->tokens[count.j] = ft_calloc(sizeof(char), token_size(str, count.i) + 1);
		count.k = 0;
		count.m = count.i;
		while (str[count.i] && count.j < minishell->lexer->num_of_tokens)
		{
			if (str[count.i] == '\"')
			{
				count.i++;
				while (str[count.i] && str[count.i] != '\"')
				{
					minishell->lexer->tokens[count.j][count.k++] = str[count.i++];
					count.m++;
				}
				count.i++;
			}
			else if (str[count.i] == '\'')
			{
				count.i++;
				while (str[count.i] && str[count.i] != '\'')
				{
					minishell->lexer->tokens[count.j][count.k++] = str[count.i++];
					count.m++;
				}
				count.i++;
			}
			else if (str[count.i] == '<' || str[count.i] == '>' || str[count.i] == '|')
				break ;
			else if (str[count.i] != ' ' && str[count.i] != '\t')
			{
				minishell->lexer->tokens[count.j][count.k++] = str[count.i++];
				count.m++;
			}
			else if (str[count.i] == ' ' || str[count.i] == '\t')
				break ;
		}
		if (count.k > 0)
			minishell->lexer->tokens[count.j++][count.k] = '\0';
		else if (count.k == 0 && count.i > count.m)
			minishell->lexer->tokens[count.j++][count.k] = '\0';
		if ((str[count.i] == '>' && str[count.i + 1] && str[count.i + 1] == '>') || (str[count.i] == '<' && str[count.i + 1] && str[count.i + 1] == '<'))
		{
			if (minishell->lexer->tokens[count.j])
			{
				free(minishell->lexer->tokens[count.j]);
				minishell->lexer->tokens[count.j] = NULL;
			}
			minishell->lexer->tokens[count.j] = malloc(sizeof(char) * (3));
			minishell->lexer->tokens[count.j][0] = str[count.i];
			minishell->lexer->tokens[count.j][1] = str[count.i + 1];
			minishell->lexer->tokens[count.j++][2] = '\0';
			count.i = skip_symbols(str, count.i);
		}
		else if (str[count.i] == '>' || str[count.i] == '<' || str[count.i] == '|')
		{
			if (minishell->lexer->tokens[count.j])
			{
				free(minishell->lexer->tokens[count.j]);
				minishell->lexer->tokens[count.j] = NULL;
			}
			minishell->lexer->tokens[count.j] = malloc(sizeof(char) * (2));
			minishell->lexer->tokens[count.j][0] = str[count.i];
			minishell->lexer->tokens[count.j++][1] = '\0';
			count.i = skip_symbols(str, count.i);
		}
		count.i = skip_spaces(str, count.i);
	}
	minishell->lexer->tokens[count.j] = NULL;
	return (minishell);
}
	// printf("num of tokens = %i\n", minishell->lexer->num_of_tokens);
		// printf("Token size = %i\n", token_size(str, count.i));
		// printf("token size in lexer = %i\n", token_size(str, count.i));
	// 	printf("tokens[%i] = %s\n", i, minishell->lexer->tokens[i]);
	// for (int i = 0; minishell->lexer->tokens[i]; i++)
	// 	printf("tokens[%i] = %s\n", i, minishell->lexer->tokens[i]);

/* LEXER DOUBLE QUOTES
** The way I am doing my lexer is in 3 steps
1) If I encounter a word outside any quotes, I add it to minishell->lexer->tokens[x];
2) If I encounter any quotes (single or double), I save all input inside quotes in
minishell->lexer->dquotes_tokens or in minishell->lexer->squotes_tokens then add it to
minishell->lexer->tokens later on by splitting
3) I finalize by freeing dquotes_tokens and squotes_tokens and passing the main tokens
to the parser
*/
// char	*lexer_dquotes(t_shell_s *minishell, char *str, int i)
// {
// 	t_counter	count;

// 	if (!minishell)
// 		return (NULL);
// 	if (!str)
// 		return (minishell);
// 	count.i = 0;
// 	count.j = 0;
// 	count.k = 0;
	// minishell->lexer->num_of_dquotes = number_of_dquotes(str);
	// if (minishell->lexer->num_of_dquotes % 2 == 1)
	// 	return (printf("Minishell: Error: lexer--utils3.c: Odd number of quotes\n"), NULL);
// 	if (str[i] == '\"')
// 		i++;
// 	minishell->lexer->dquotes_tokens = malloc(sizeof(char *) * minishell->lexer->num_of_dquotes);
// 	while (str[count.i])
// 	{
// 		if (str[count.i] == '\"')
// 		{
// 			count.i++;
// 			minishell->lexer->dquotes_tokens[count.j] = malloc(sizeof(char) * (ft_strlen_dquotes(str, count.i) + 1));
// 			minishell->lexer->dquotes_tokens[count.j++] = dqouted_string(str, count.i);
// 		}
// 	}
// 	return (minishell);
// }

// char	*lexer_dquotes(t_shell_s *minishell, char *str, int i)
// {
// 	t_counter	count;
// 	char		*word;

// 	if (!str || !minishell)
// 		return (NULL);
// 	count.j = 0;
// 	word = malloc(sizeof(char) * ft_strlen_dquotes(str, i) + 1);
// 	while (str[i] && (str[i] != ' ' && str[i] != '\t' && str[i] != '\"'))
// 	{
// 		word[count.j++] = str[i++];
// 	}
// 	word[count.j] = '\0';
// 	return (word);
// }

// char	*dqouted_string(char *str, int i)
// {
// 	char	*dquoted_str;
// 	int		j;

// 	if (!str)
// 		return (NULL);
// 	if (str[i] == '\"')
// 		i++;
// 	j = 0;
// 	dquoted_str = malloc(sizeof(char) * ft_strlen_dquotes(str, i) + 1);
// 	while (str[i] && str[i] != '\"')
// 		dquoted_str[j++] = str[i++];
// 	return (dquoted_str);
// }

int	ft_strlen_dquotes(char *str, int i)
{
	int	length;
	int	trigger;

	length = 0;
	trigger = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			trigger = 1;
		if ((str[i] == ' ' || str[i] == '\t') && trigger == 1)
			break;
		length++;
		i++;
	}
	length--;
	// printf("length = %i\n", length);
	return (length);
}

int	ft_strlen_squotes(char *str, int i)
{
	int	length;
	int	trigger;

	length = 0;
	trigger = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			trigger = 1;
		if ((str[i] == ' ' || str[i] == '\t') && trigger == 1)
			break;
		length++;
		i++;
	}
	length--;
	// printf("length = %i\n", length);
	return (length);
}

	// int	length;

	// length = 0;
	// while (str[i] && (str[i] != '\''))
	// {
	// 	length++;
	// 	i++;
	// }
	// return (length);

int	number_of_dquotes(char *str)
{
	t_counter	count;

	count.i = 0;
	count.counter = 0;
	count.trigger = 0;
	while(str[count.i])
	{
		if (str[count.i] == '\'' && count.counter % 2 == 0)
		{
			if (count.trigger == 0)
				count.trigger = 1;
			else
				count.trigger = 0;
		}
		if (str[count.i] == '\"' && count.trigger == 0)
			count.counter++;
		count.i++;
	}
	return (count.counter);
}

int	number_of_squotes(char *str)
{
	t_counter	count;

	count.i = 0;
	count.counter = 0;
	count.trigger = 0;
	while(str[count.i])
	{
		if (str[count.i] == '\"' && count.counter % 2 == 0)
		{
			if (count.trigger == 0)
				count.trigger = 1;
			else
				count.trigger = 0;
		}
		if (str[count.i] == '\'' && count.trigger == 0)
			count.counter++;
		count.i++;
	}
	return (count.counter);
}

// int	number_of_squotes(char *str)
// {
// 	int	i;
// 	int	counter;

// 	i = 0;
// 	counter = 0;
// 	while(str[i])
// 	{
// 		if (str[i] == '\'')
// 			counter++;
// 		i++;
// 	}
// 	return (counter);
// }

void	print_struct(t_shell_s *args)
{

	t_counter	counter;

	counter.i = 0;
	counter.j = 0;
	counter.k = 0;
	counter.m = 0;
	counter.n = 0;
	printf("\nNUMBER OF COMMANDS: %i\n", args->num_commands);
	printf("\nNUMBER OF PIPES: %i\n", args->num_pipes);
	printf("\nPRINTING COMMANDS:\n");
	while (counter.i < args->num_commands)
		printf("%s\n", args->commands[counter.i++]);
	// printf("\nPRINTING RAW COMMANDS:\n");
	// while (counter.n < args->num_commands)
	// 	printf("%s\n", args->cmd_raw[counter.n++]);
	// printf("\nPRINTING COMMANDS PATH:\n");
	// while (counter.j < args->num_commands)
	// 	printf("%s\n", args->cmd_path[counter.j++]);
	// printf("\nPRINTING COMMANDS WITH NO FLAGS:\n");
	// while (counter.k < args->num_commands)
	// 	printf("%s\n", args->cmd_noflag[counter.k++]);
	// printf("\nPRINTING FULL PATH:\n%s\n", args->full_path);
	printf("\nPRINTING HOME:\n");
	printf("\nPRINTING SPLIT PATHS:\n");
	while (args->path[counter.m])
		printf("%s\n", args->path[counter.m++]);
	printf("\n\n");
	return ;
}