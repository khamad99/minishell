/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/06/06 18:00:59 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	export_init(t_shell_s *shell, int flag)
{
	int	i;

	if (flag != 0 || !shell->envp->envp || !shell->envp->envp[0])
		return ;
	i = -1;
	shell->envp->export_env = (char **)ft_calloc(ft_strstrlen(
				shell->envp->envp), sizeof(char *) + 1);
	shell->envp->export_key = (char **)ft_calloc(ft_strstrlen(
				shell->envp->key), sizeof(char *) + 1);
	shell->envp->export_value = (char **)ft_calloc(ft_strstrlen(
				shell->envp->value), sizeof(char *) + 1);
	while (shell->envp->envp[++i])
	{
		shell->envp->export_env[i] = ft_strdup(shell->envp->envp[i]);
		shell->envp->export_key[i] = ft_strdup(shell->envp->key[i]);
		shell->envp->export_value[i] = ft_strdup(shell->envp->value[i]);
	}
	shell->envp->export_env[i] = 0;
	shell->envp->export_key[i] = 0;
	shell->envp->export_value[i] = 0;
}

int	check_in_export(t_env_s *env, char *str, int *flag)
{
	int		i;
	char	*key;

	get_flag(flag, str);
	if (*flag != 0)
	{
		key = ft_calloc(*flag + 1, sizeof(char));
		ft_strlcpy(key, str, *flag + 1);
		i = -1;
		while (env->export_key[++i])
		{
			if (!ft_strncmp(key, env->export_key[i], ft_strlen(key)))
			{
				free(key);
				return (i);
			}
		}
		free(key);
		return (-1);
	}
	return (-1);
}

int	ft_export(t_execute *cmd)
{	
	int		i;
	int		p;
	int		flag;

	if (!cmd->env->envp || !cmd->env->envp[0])
		return (EXIT_SUCCESS);
	i = 0;
	if (!cmd->args[1])
	{
		env_export_printing(cmd->env);
		return (0);
	}
	else
	{
		while (cmd->args[++i])
		{
			p = check_in_export(cmd->env, cmd->args[i], &flag);
			if (export_args_check(cmd->args[i]) == 0 && p == -1)
				return (exit_export(cmd->args[i]), EXIT_FAILURE);
			else
				ft_export2(cmd, &flag, p, i);
		}
	}
	return (EXIT_SUCCESS);
}
