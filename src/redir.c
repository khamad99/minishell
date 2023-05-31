/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 13:32:12 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/30 20:13:18 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
counter struct used to declare the counter variables to save space in functions
*/
typedef struct counter
{
	int	i;
	int	infile_i;
	int	append_i;
	int	outfile_i;
	int	hd_i;
}	counter;

/*
this function used to compute the lenth of char ** variable 
it is used to count the number of each redir in the char **names
*/
int	ft_strstrlen(char **str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

/*
this function have 2 roles:
1- call the heredoc function first to excute the heredoc at the begen
2- calles the open_functoin for each redirection to open / 
close the files (if onther files with the same type is provided)
it will return -1 if no access to infile 
*/
static int	open_files(t_files *files, t_shell_s *shell)
{
	(void)shell;
	counter	c;

	c.infile_i = -1;
	c.append_i = -1;
	c.outfile_i = -1;
	c.i = -1;
	while (files->redirect_type[++c.i])
	{
		if (files->redirect_type[c.i] == '>')
		{
			if (open_outfile(files, ++c.outfile_i) == -1)
				return (-1);
		}
		else if (files->redirect_type[c.i] == 'a')
		{
			if (open_appendfile(files, ++c.append_i) == -1)
			return (-1);
		}
		else if (files->redirect_type[c.i] == '<')
		{
			++c.infile_i;
			if (access(files->infile_name[c.infile_i], F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(files->infile_name[c.infile_i], STDERR_FILENO);
				ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
				return (-1);
			}
			open_infile(files, c.infile_i);
		}
	}
	return (0);
}

/*
to minimise the init_redir function 
*/
static void	init_redir2(t_execute *cmd, counter *c)
{
	if (cmd->files->redirect_type[c->i] == '>')
	{
		dup2(cmd->files->outfile_fd, STDOUT_FILENO);
		if (++c->outfile_i == ft_strstrlen(cmd->files->outfile_name))
			close(cmd->files->outfile_fd);
	}
	else if (cmd->files->redirect_type[c->i] == 'a')
	{
		dup2(cmd->files->append_fd, STDOUT_FILENO);
		if (++c->append_i == ft_strstrlen(cmd->files->append_name))
			close(cmd->files->append_fd);
	}
	else if (cmd->files->redirect_type[c->i] == '<')
	{
		dup2(cmd->files->infile_fd, STDIN_FILENO);
		if (++c->infile_i == ft_strstrlen(cmd->files->infile_name))
			close(cmd->files->infile_fd);
	}
}

/*
this function used to call the open fds and then duplicate the fd
accourding to the order of redir in cmd_block and closes the fd in the
final match of each redir_type
it will return -1 if no access to infile
*/
int	init_redir(t_execute *cmd, t_shell_s *shell)
{
	counter c;

	c.i = -1;
	c.infile_i = 0;
	c.outfile_i = 0;
	c.append_i = 0;
	c.hd_i = 0;
	if (open_files(cmd->files, shell) == -1)
		return (-1);
	while (cmd->files->redirect_type[++c.i])
	{
		init_redir2(cmd, &c);
		if (cmd->files->redirect_type[c.i] == 'h')
		{
			cmd->files->heredoc_fd = open(".temp", O_RDONLY);
			if (cmd->files->heredoc_fd == -1)
				return (-1);
			dup2(cmd->files->heredoc_fd, STDIN_FILENO);
			if (++c.hd_i == ft_strstrlen(cmd->files->limiter))
				close(cmd->files->heredoc_fd);
		}
	}
	return (0);
}

int	init_heredoc(t_execute *cmd, t_shell_s *shell)
{
	// int	i;
	// int	hd_i;

	// i = -1;
	open_exec_heredoc(cmd->files, shell);
	// while (cmd->files->redirect_type[++i])
	// {
	// 	if (cmd->files->redirect_type[i] == 'h')
	// 	{
	// 		open("temp", O_RDONLY);
	// 		dup2(cmd->files->heredoc_fd, STDIN_FILENO);
	// 		if (++hd_i == ft_strstrlen(cmd->files->limiter))
	// 			close(cmd->files->heredoc_fd);
	// 	}
	// }
	return (0);
}
