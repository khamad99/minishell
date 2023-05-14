/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 20:03:04 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/13 20:03:41 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "export", 7))
		return (1);
	else if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	else if (!ft_strncmp(cmd, "echo", 5))
		return (1);
	else if (!ft_strncmp(cmd, "env", 4))
		return (1);
	else if (!ft_strncmp(cmd, "unset", 6))
		return (1);
	else if (!ft_strncmp(cmd, "exit", 5))
		return (1);
	else if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	return (0);
}

int	builtin_exec(t_execute *exec)
{
	// if (!ft_strncmp(exec->command, "export", 7))
	// 	return (built_in_export(shell, cmd_args));
	// else if (!ft_strncmp(exec->command, "cd", 3))
	// 	return (ft_cd(shell, cmd_args));
	if (!ft_strncmp(exec->command, "echo", 5))
		return (ft_echo(exec->args));
	else if (!ft_strncmp(exec->command, "env", 4))
		return (ft_env(exec->args, exec->env));
	// else if (!ft_strncmp(exec->command, "unset", 6))
	// 	return (built_in_unset(shell, cmd_args));
	// else if (!ft_strncmp(exec->command, "exit", 5))
	// 	built_in_exit(shell, cmd_args);
	// else if (!ft_strncmp(cexec->command, "pwd", 4))
	// 	return (built_in_pwd(shell, cmd_args[0]));
	return (0);
}