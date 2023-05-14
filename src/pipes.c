/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 21:42:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/13 22:11:56 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pipes_pid_init(t_shell_s *shell)
{
	int		i;

	if (shell->num_pipes == 0)
		return;
	shell->pipes_fd = (int *)ft_calloc(shell->num_pipes * 2 , sizeof(int));
	if (!shell->pipes_fd)
		return ; // fix the free function later
	i = -1;
	while (++i < shell->num_pipes)
	{
		if (pipe(shell->pipes_fd + (i * 2)) == -1)
			return; // fix the free function later
		i++;
	}
	shell->pid = (pid_t *)ft_calloc(shell->num_pipes + 1, sizeof(pid_t));
	if (!shell->pid)
		return; // fix the free function later + message Unable to allocate memory
}