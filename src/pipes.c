/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 21:42:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/22 22:00:05 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pipes_pid_init(t_shell_s *shell)
{
	int		i;

	shell->pid = (pid_t *)ft_calloc(shell->num_commands, sizeof(pid_t));
	if (!shell->pid)
		return; // fix the free function later + message Unable to allocate memory
	if (shell->num_pipes <= 0)
		return ;
	shell->pipes_fd = (int *)ft_calloc(4 ,sizeof(int));
	if (!shell->pipes_fd)
		return ; // fix the free function later
	i = -1;
	while (++i < 2)
		if (pipe(shell->pipes_fd + (i * 2)) == -1)
			return ; // fix the free function later
	
	
	
	// int		i;

	// shell->pid = (pid_t *)ft_calloc(shell->num_commands, sizeof(pid_t));
	// if (!shell->pid)
	// 	return; // fix the free function later + message Unable to allocate memory
	// if (!shell->num_pipes)
	// 	return ;
	// shell->pipes_fd = (int *)ft_calloc(shell->num_pipes * 2 ,sizeof(int));
	// if (!shell->pipes_fd)
	// 	return ; // fix the free function later
	// i = -1;
	// while (++i < shell->num_pipes)
	// {
	// 	if (pipe(shell->pipes_fd + (i * 2)) == -1)
	// 		return ; // fix the free function later
	// }
}

static void	pipes_in_child_odd(t_shell_s *shell, int cmd_num)
{
	close(shell->pipes_fd[1]);
	close(shell->pipes_fd[2]);
	if (cmd_num == shell->num_pipes)
		dup2(shell->pipes_fd[0], STDIN_FILENO);
	else
	{
		dup2(shell->pipes_fd[3], STDOUT_FILENO);
		dup2(shell->pipes_fd[0], STDIN_FILENO);
	}
	close(shell->pipes_fd[0]);
	close(shell->pipes_fd[3]);
}

/*
this function called in child process, it closes all fds that not related 
to this child and change the defult fd in the child accourding to pipes
*/

void	pipes_in_child(t_shell_s *shell, int cmd_num)
{
	if (shell->num_pipes <= 0)
		return ;
	if (cmd_num % 2 == 0)
	{
		close(shell->pipes_fd[0]);
		close(shell->pipes_fd[3]);
		if (cmd_num == 0)
			dup2(shell->pipes_fd[1], STDOUT_FILENO);
		if (cmd_num == shell->num_pipes)
			dup2(shell->pipes_fd[2], STDIN_FILENO);
		else
		{
			dup2(shell->pipes_fd[1], STDOUT_FILENO);
			dup2(shell->pipes_fd[2], STDIN_FILENO);
		}
		close(shell->pipes_fd[2]);
		close(shell->pipes_fd[1]);
	}
	else if (cmd_num % 2 != 0)
		pipes_in_child_odd(shell, cmd_num);

	
	// int	i;

	// i = -1;
	// while (++i < shell->num_pipes * 2)
	// 	if (((cmd_num * 2) - 2) != i && ((cmd_num * 2) + 1) != i)
	// 		close(shell->pipes_fd[i]);
	// if (cmd_num == 0)
	// {
	// 	dup2(shell->pipes_fd[1], STDOUT_FILENO);
	// 	close(shell->pipes_fd[1]);
	// }
	// else if (cmd_num == shell->num_pipes)
	// {
	// 	dup2(shell->pipes_fd[(cmd_num * 2) - 2], STDIN_FILENO);
	// 	close(shell->pipes_fd[(cmd_num * 2) - 2]);
	// }
	// else
	// {
	// 	dup2(shell->pipes_fd[(cmd_num * 2) - 2], STDIN_FILENO);
	// 	close(shell->pipes_fd[(cmd_num * 2) - 2]);
	// 	dup2(shell->pipes_fd[(cmd_num * 2) + 1], STDOUT_FILENO);
	// 	close(shell->pipes_fd[(cmd_num * 2) + 1]);
	// }
}
