/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 12:21:06 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/30 23:10:55 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

/* The function serves two purposes:

1- If one or more arguments are provided, it only considers the first argument. 
It utilizes the "stat" function, which refers to a structure
called "struct stat", to determine whether the provided string corresponds 
to an existing file or folder path. In either case, it returns the value 126.

2- If no argument is given, the function prints all environment variables and their 
respective values to the standard output (stdout). Each variable and its value are 
displayed on a separate line, with a newline character ("\n") following each line .
*/

static void	env_error(int code, char *arg)
{
	if (code == 1)
	{
		ft_putstr_fd("env: ‘", STD_ERROR);
		ft_putstr_fd(arg, STD_ERROR);
		ft_putstr_fd("’: No such file or directory\n", STD_ERROR);
	}
	if (code == 2)
	{
		ft_putstr_fd("env: ‘", STD_ERROR);
		ft_putstr_fd(arg, STD_ERROR);
		ft_putstr_fd("’: Permission denied\n", STD_ERROR);
	}
}

int	ft_env(char ** arg, t_env_s *env)
{
	int			re;
	struct stat	env_stat;
	int			i;

	if (arg[1])
	{
		re = stat(arg[1], &env_stat);
		if (S_ISREG(env_stat.st_mode) || re != 0)
			env_error(1, arg[1]);
		else if (S_ISDIR(env_stat.st_mode))
			env_error(2, arg[1]);
		return (126);
	}
	i = -1;
	while (env->envp[++i])
	{
		ft_putstr_fd(env->envp[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (0);
}