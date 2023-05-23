/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 08:08:08 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/24 00:46:52 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_next_line(int fd)
{
	int		i;
	int		rd;
	char	c;
	char	*buffer; 
	
	buffer = malloc(100000);
	rd = 0;
	i = 0;
	while ((rd = read(fd, &c, 1)) > 0)
	{
		buffer[i++] = c;
		if (c == '\n')
		{
			buffer[i - 1] =  '\0';
			break ;
		}
	}
	if (rd == -1 || i == 0 || (!buffer[i - 1] && !rd))
		return (free(buffer), NULL);
	return(buffer);
}

static void	exec_heredoc(t_files *files, t_shell_s *shell)
{
	char	*input;
	int		i;

	i = -1;
	while (files->limiter[++i])
	{
		while (1)
		{
			write(shell->std_out, "> ", 2);
			input = get_next_line(shell->std_in);
			if (!input)
			{
				write(shell->std_out, "^D\n", 3);	
				break ;
			}
			add_history(input);
			if (!ft_strncmp(files->limiter[i], input, ft_strlen(files->limiter[i]) + 1))
			{
				free(input);
				break ;
			}
			if (!files->limiter[i + 1])
			{
				ft_putstr_fd(input, files->heredoc_fd);
				ft_putstr_fd("\n", files->heredoc_fd);
			}
			free(input);
			input = 0;
		}
	}
	close(files->heredoc_fd);
}

/*
O_CREAT: This flag specifies that the file should be created if 
it doesn't exist.
O_TRUNC: This flag specifies that the file should be truncated 
(its contents deleted) if it already exists.
O_WRONLY flag specifies that the file should be opened for writing only
0644: This is the file mode that specifies the permissions for the newly created file
*/
void	open_exec_heredoc(t_files *files, t_shell_s *shell)
{
	if (files->limiter)
	{
		files->heredoc_fd = open("temp", 
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		exec_heredoc(files, shell);
	}
}