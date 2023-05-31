/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/31 16:50:00 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



void add_export_args(char *str, int *flag)
{
	if (*flag == 1)
	{
		//add to env and export
	}
	else
		//add to export only
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

/*
1 -> do nothing
0 -> not valid
2 -> ok to add
*/

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
static int	export_args_check(char *str, int *flag)
{
	int		i;
	char	r[10];

	ft_strlcpy(r, " !@$%^-\\", 10);
	i = -1;
	*flag = 0;
	if (!ft_strncmp(str, "_\0", 2))
		return (1);
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
			*flag = 1;
		else if (flag == 0 && ft_strchr(r, str[i]) != NULL)
			return (0);
		else if (*flag == 1 && !str[i + 1])
			return (2);
	}
	return (0);
}

static int	check_in_export(t_env_s *env, char *str, int *flag)
{
	int		i;
	char	*key;
	
	i = -1;
	*flag = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
		{
			*flag = i;
			break ;
		}
	}
	if (*flag != 0)
	{
		key = ft_calloc(*flag, sizeof(char) + 1);
		ft_strlcpy(key, str, *flag);
		i = -1;
		while (env->export_key[++i])
		{
			if (!ft_strncmp(key, env->export_key[i], ft_strlen(str)))
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

static int	check_in_env(t_env_s *env, char *str, int *flag)
{
	int		i;
	char	*key;
	
	i = -1;
	*flag = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
		{
			*flag = i;
			break ;
		}
	}
	if (*flag != 0)
	{
		key = ft_calloc(*flag, sizeof(char) + 1);
		ft_strlcpy(key, str, *flag);
		i = -1;
		while (env->key[++i])
		{
			if (!ft_strncmp(key, env->key[i], ft_strlen(str)))
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

void	add_value_to_env(int p, t_env_s *env, char *str, int *flag)
{
	free(env->envp[p]);
	env->envp[p] = ft_calloc(ft_strlen(str), sizeof(char));
	ft_strlcpy(env->envp[p], str, ft_strlen(str));
	free(env->value);
	env->envp[p] = ft_calloc(ft_strlen(str) - *flag, sizeof(char));
	ft_strlcpy(env->value[p], str + *flag, ft_strlen(str) - *flag);
}

void	add_value_to_export(int p, t_env_s *env, char *str, int *flag)
{
	free(env->export_env[p]);
	env->export_env[p] = ft_calloc(ft_strlen(str), sizeof(char));
	ft_strlcpy(env->export_env[p], str, ft_strlen(str));
	free(env->value);
	env->envp[p] = ft_calloc(ft_strlen(str) - *flag, sizeof(char));
	ft_strlcpy(env->value[p], str + *flag, ft_strlen(str) - *flag);
}

int	ft_export(t_execute *cmd)
{	
	int		i;
	int		p;
	int		flag;

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
			p = check_in_export(cmd->env, cmd->args[i], &flag);
			if (p != -1)
			{
				add_value_to_export(p, cmd->env, cmd->args[i], &flag);
				p = check_in_env(cmd->env, cmd->args[i], &flag);
				if (p != -1)
					add_value_to_env(p, cmd->env, cmd->args[i], &flag);
			}
			else if (export_args_check(cmd->args[i], &flag))
				add_export_args(cmd->args[i], &flag);
			else
			{
				ft_putstr_fd("minishell: export: ", STDERR_FILENO);
				ft_putstr_fd(cmd->args[i], STDERR_FILENO);
				ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
			}
		}
		return (1);
	}
	return (0);
}
