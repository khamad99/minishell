/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:47:48 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/16 10:10:23 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
used the remove the env from all arrays in t_env_s stract
*/
static void	remove_env_from_list(t_env_s *env, int position)
{
	env->env_size--;
	while (env->envp[position] && env->key[position] && env->value[position])
	{
		env->envp[position] = env->envp[position + 1];
		env->key[position] = env->key[position + 1];
		env->value[position] = env->value[position + 1];
		position++;
	}
	env->envp[position] = 0;
	env->key[position] = 0;
	env->value[position] = 0;
	// int i = -1;
	// while (env->envp[++i])
	// 	printf("%s\n", env->envp[i]);
	
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
		position = unset_arg_comp(exec->args[i], exec->env->key);
		if (position != -1)
			remove_env_from_list(exec->env, position);
	}
	return (0);
}