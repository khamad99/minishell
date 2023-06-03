/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:47:48 by kalshaer          #+#    #+#             */
/*   Updated: 2023/06/03 10:22:12 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	unset_args_check(char *str)
{
	int		i;
	char	r[19];

	ft_strlcpy(r, " !@$%^-\\+{}*#~.=;", 19);
	i = -1;
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	while (str[++i] != '\0')
	{
		if (ft_strchr(r, str[i]) != NULL)
			return (0);
		else if (!str[i + 1])
			return (1);
	}
	return (0);
}

/*
used the remove the env from all arrays in t_env_s stract
*/
static void	remove_env_from_list(t_env_s *env, int position)
{
	env->env_size--;
	free(env->envp[position]);
	free(env->key[position]);
	while (env->envp[position] && env->key[position])
	{
		env->envp[position] = env->envp[position + 1];
		env->key[position] = env->key[position + 1];
		position++;
	}
	env->envp[position] = 0;
	env->key[position] = 0;
}

static void	remove_export_from_list(t_env_s *env, int position)
{
	free(env->export_key[position]);
	free(env->export_value[position]);
	while (env->export_key[position])
	{
		env->export_key[position] = env->export_key[position + 1];
		env->export_value[position] = env->export_value[position + 1];
		position++;
	}
	env->export_key[position] = 0;
	env->export_value[position] = 0;
}

/*
used to locate the env_key position in the array
*/
static int	unset_arg_comp(char *arg, char **env_key)
{
	int i;

	i = -1;
	while (env_key[++i])
	{
		if (!ft_strncmp(arg, env_key[i], ft_strlen(arg)))
			return (i);
	}
	return (-1);
}

static int	export_arg_comp(char *arg, char **env_key)
{
	int i;

	i = -1;
	while (env_key[++i])
	{
		if (!ft_strncmp(arg, env_key[i], ft_strlen(arg)))
			return (i);
	}
	return (-1);
}

/*
remainig error handle 
*/
int	ft_unset(t_execute *exec)
{
	int		i;
	int		position;

	i = 0;
	while (exec->args[++i])
	{
		if (unset_args_check(exec->args[i]) == 0)
		{
			ft_putstr_fd("minishell: unset: ", STDERR_FILENO);
			ft_putstr_fd(exec->args[i], STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		position = unset_arg_comp(exec->args[i], exec->env->key);
		if (position != -1)
			remove_env_from_list(exec->env, position);
		position = export_arg_comp(exec->args[i], exec->env->export_key);
		if (position != -1)
			remove_export_from_list(exec->env, position);
	}
	return (EXIT_SUCCESS);
}