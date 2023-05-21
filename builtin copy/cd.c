/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:09 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/21 11:09:58 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	add_pwd_env(t_env_s *env)
{
	(void)env;
	char	*pwd;

	if (!getcwd(NULL, 0))
		return ; // error handling
	pwd = ft_strjoin("PWD=", getcwd(NULL, 0));
	// OLD_PWD = PWD
	// add new pwd to env
	free(pwd);
}

int	ft_cd(t_execute *cmd)
{
	int 	i;
	char	*path;

	i = -1;
	if (cmd->args[2])
		return 1;
		//return too many argument
	if (!ft_strncmp(cmd->args[1], ".", 2))
		return 1;
		//return do nothing
	if (!cmd->args[1] || ft_strncmp(cmd->args[1], "~", 2))
	{
		while (cmd->env->key[++i])
			if (!ft_strncmp(cmd->env->key[i], "HOME", 5))
			{
				path = cmd->env->value[i];
				break;
			}
	}
	if (!ft_strncmp(cmd->args[1], "-", 2))
	{
		while (cmd->env->key[++i])
			if (!ft_strncmp(cmd->env->key[++i], "OLDPWD", 7))
			{
				path = cmd->env->value[i];
				break;
			}
	}
	path = cmd->args[1];
	if (!chdir(path))
		add_pwd_env(cmd->env);
	else
		return (1); // error handle
	return (0);
}
