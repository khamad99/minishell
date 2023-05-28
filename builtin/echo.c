/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 13:09:07 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/28 13:44:15 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static void	ft_putstr_fd_echo(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] == 92 && s[i + 1])
// 			i++;
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// }

static int	nb_args(char **args)
{
	int	size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}

int	ft_echo(char **args)
{
	int	i;
	int	i2;
	int	j;
	int	n_option;

	i = 1;
	n_option = 0;
	if (nb_args(args) > 1)
	{
		i2 = 0;
		while(args[++i2])
		{
			if (ft_strncmp(args[i2], "-n", 2) == 0)
			{
				n_option = 1;
				j = 0;
				while (args[1][++j])
				{
					if (args[1][j] != 'n')
						n_option = 0;
				}
			}
		}
		if (n_option)
			i = i2 - 1;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1] && args[i][0] != '\0')
				write(1, " ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
