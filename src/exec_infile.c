/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_infile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:49:42 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/30 20:09:02 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
O_RDWR: This flag specifies that the file should be opened for both reading 
and writing.
*/
void	open_infile(t_files *files, int i)
{
	files->infile_fd = open(files->infile_name[i], O_RDONLY);
	if (i < ft_strstrlen(files->infile_name) - 1)
		close(files->infile_fd);
}
