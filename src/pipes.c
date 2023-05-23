/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 21:42:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/24 00:49:17 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pid_pipes_init(t_shell_s *shell)
{
	int		i;

	shell->pid = (pid_t *)ft_calloc(shell->num_commands, sizeof(pid_t));
	if (!shell->pid)
		return; // fix the free function later + message Unable to allocate memory
	if (!shell->num_pipes)
		return ;
	shell->pipes_fd = (int *)ft_calloc(shell->num_pipes * 2 ,sizeof(int));
	if (!shell->pipes_fd)
		return ; // fix the free function later
	i = -1;
	while (++i < shell->num_pipes)
	{
		if (pipe(shell->pipes_fd + (i * 2)) == -1)
			return ; // fix the free function later
	}
}

void	pipes_in_child(t_shell_s *shell, int cmd_num)
{
	int	i;

	if (shell->num_pipes <= 0)
		return ;
	i = -1;
	while (++i < shell->num_pipes * 2)
		if (((cmd_num * 2) - 2) != i && ((cmd_num * 2) + 1) != i)
			close(shell->pipes_fd[i]);
	if (cmd_num == 0)
	{
		dup2(shell->pipes_fd[1], STDOUT_FILENO);
		close(shell->pipes_fd[1]);
	}
	else if (cmd_num == shell->num_pipes)
	{
		dup2(shell->pipes_fd[(cmd_num * 2) - 2], STDIN_FILENO);
		close(shell->pipes_fd[(cmd_num * 2) - 2]);
	}
	else
	{
		dup2(shell->pipes_fd[(cmd_num * 2) - 2], STDIN_FILENO);
		close(shell->pipes_fd[(cmd_num * 2) - 2]);
		dup2(shell->pipes_fd[(cmd_num * 2) + 1], STDOUT_FILENO);
		close(shell->pipes_fd[(cmd_num * 2) + 1]);
	}
}
