/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:09 by kalshaer          #+#    #+#             */
/*   Updated: 2023/06/01 22:36:21 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	add_pwd_env(t_env_s *env)
{
	char	*pwd;
	char	*pwd_export;
	int		i;
	int		old_pwd_p;
	int		old_pwd_p_export;

	i = -1;
	while (env->key[++i])
		if (!ft_strncmp(env->key[i], "OLDPWD", 7))
			old_pwd_p = i;
	i = -1;
	while (env->export_key[++i])
		if (!ft_strncmp(env->export_key[i], "OLDPWD", 7))
			old_pwd_p_export = i;
	if (!getcwd(NULL, 0))
		return ; // error handling
	pwd = ft_strjoin("PWD=", getcwd(NULL, 0));
	pwd_export = ft_strdup(pwd);
	i = -1;
	while (env->key[++i])
	{
		if (!ft_strncmp(env->key[i], "PWD", 4))
		{
			free(env->envp[old_pwd_p]);
			env->envp[old_pwd_p] = ft_strjoin("OLD", env->envp[i]);
			free(env->envp[i]);
			env->envp[i] = pwd;
		}
	}
	i = -1;
	while (env->export_key[++i])
	{
		if (!ft_strncmp(env->export_key[i], "PWD", 4))
		{
			free(env->export_value[old_pwd_p_export]);
			env->export_value[old_pwd_p_export] = ft_strdup(env->export_value[i]);
			free(env->export_value[i]);
			env->export_value[i] = pwd_export;
		}
	}
	if (!pwd)
		free(pwd);
	if (!pwd_export)
		free(pwd_export);

}

int	ft_cd(t_execute *cmd)
{
	int 	i;
	char	*path;

	path = 0;
	i = -1;
	if (ft_strstrlen(cmd->args) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return 1;
	}
	if (!cmd->args[1] || !ft_strncmp(cmd->args[1], "~\0", 2))
	{
		while (cmd->env->key[++i])
			if (!ft_strncmp(cmd->env->key[i], "HOME", 4))
			{
				path = cmd->env->value[i];
				break;
			}
	}
	else if (!ft_strncmp(cmd->args[1], ".", 2))
		return 0;
	else if (!ft_strncmp(cmd->args[1], "-", 2))
	{
		while (cmd->env->key[++i])
			if (!ft_strncmp(cmd->env->key[++i], "OLDPWD", 7))
			{
				path = cmd->env->value[i];
				break;
			}
	}
	if (!path)
		path = cmd->args[1];
	if (!chdir(path))
		add_pwd_env(cmd->env);
	else
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return 1;
	}
	return (0);
}
