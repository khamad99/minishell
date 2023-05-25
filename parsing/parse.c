/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 11:29:43 by ooutabac          #+#    #+#             */
/*   Updated: 2023/05/25 15:16:21 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_shell_s	*parse(t_shell_s *minishell, char *str, char **envp, int iteration_num)
{
	// t_shell_s	*minishell;
	char		*expanded_str;

	if (iteration_num == 0)
		minishell = ft_calloc(sizeof(t_shell_s), 1);
	minishell->pid = 0;
	minishell->pipes_fd = 0;
	minishell->cmd_used = -1;
	minishell->std_in = dup(STDIN_FILENO);
	minishell->std_out = dup(STDOUT_FILENO);
	if (number_of_dquotes(str) % 2 == 1 || number_of_squotes(str) % 2 == 1)
	{
		// printf("Minishell: Error: lexer: Odd number of quotes\n");
		ft_putstr_fd("Minishell: Error: lexer: Odd number of quotes\n", STDERR_FILENO);
		g_exit_code = 1;
		return (minishell);
	}
	expanded_str = dollar_sign(minishell, str);
	minishell->cmd_line = ft_strdup(str);
	if (iteration_num == 0)
	{
		minishell = get_path(minishell, envp);
		minishell = get_env_struct(minishell, envp);
	}
	minishell = lexer(minishell, expanded_str);
	minishell = raw_lexer(minishell, str);
	minishell->lexer->num_of_tokens = num_of_tokens(expanded_str);
	free(expanded_str);
	if (minishell && minishell->lexer)
		minishell->lexer->command_blocks = split_pipes(str);
	if (check_validity(minishell, str) == FALSE)
	{
		free_after_execution(minishell);
		ft_putstr_fd("Error: Syntax\n", STDERR_FILENO);
		g_exit_code = 2;
		// g_exit_code = 258;
		return (minishell);
	}
	// minishell = dollar_sign(minishell);
	if (minishell)
		minishell->num_pipes = count_pipes(str);
	minishell = get_num_commands(minishell);
	minishell = get_commands(minishell);
	minishell = get_flags(minishell);
	minishell = get_execution_blocks(minishell);
	// g_exit_code = 0;
	return (minishell);
}
