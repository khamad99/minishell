/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:27:20 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/15 23:29:43 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = ft_calloc(4096, sizeof(char));
	if (!pwd)
		return (1);
	pwd = getcwd(pwd, 4096);
	if (!pwd)
	{
		free(pwd);
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
