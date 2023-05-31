/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/30 23:17:18 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// {
// 	!;
// 	@;
// 	$;
// 	%;
// 	^;
// 	-;
// 	" ";
// 	"\";
// 	_; //->one only will do nothing if it is alone
// }

void add_export_args(char *str)
{
	(void)str;
}

static void sort_env(char **envp) 
{
	int		i;
	char	*temp;
	int		len;
	int		j;

	j = -1;
	len = ft_strstrlen(envp);
	while (++j < len)
	{
		i = -1;
		while (envp[++i])
		{
			if (envp[i + 1] && ft_strncmp(envp[i], envp[i + 1], ft_strlen(envp[i])) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
			}
		}
	}
}

static void	env_export_printing(t_env_s *env)
{
	int	i;

	sort_env(env->export_env);
	i = -1;
	while (env->export_env[++i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->export_key[i], STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->export_value[i], STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

static int	export_args_check(char *str)
{
	int	i;

	i = -1;
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	while (str[++i] != '=' && str[i] != '\0')
	{
		if (!ft_isdigit(str[i]) && str[i] != '_' && !ft_isalpha(str[i]))
			return (0);
	}
	return (1);
}

int	ft_export(t_execute *cmd)
{	
	int		i;

	i = -1;
	cmd->env->export_env = (char **)ft_calloc(ft_strstrlen(cmd->env->envp), sizeof(char *) + 1);
	cmd->env->export_key = (char **)ft_calloc(ft_strstrlen(cmd->env->key), sizeof(char *) + 1);
	cmd->env->export_value = (char **)ft_calloc(ft_strstrlen(cmd->env->value), sizeof(char *) + 1);
	while (cmd->env->envp[++i])
	{
		cmd->env->export_env[i] = ft_strdup(cmd->env->envp[i]);
		cmd->env->export_key[i] = ft_strdup(cmd->env->key[i]);
		cmd->env->export_value[i] = ft_strdup(cmd->env->value[i]);
	}
	cmd->env->export_env[i] = 0;
	cmd->env->export_key[i] = 0;
	cmd->env->export_value[i] = 0;
	i = -1;
	if (!cmd->args[1])
	{
		env_export_printing(cmd->env);
		return (0);
	}
	else
	{
		while (cmd->args[++i])
		{
			if (export_args_check(cmd->args[i]))
				add_export_args(cmd->args[i]);
			else
			{
				return (1);
			}
		}
	}
	return (0);
}
