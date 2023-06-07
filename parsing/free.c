/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 05:35:50 by ooutabac          #+#    #+#             */
/*   Updated: 2023/06/07 09:53:17 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_2d_utils(char **array)
{
	int	i;

	i = 0;
	if (array != NULL)
	{
		if (array[i])
		{
			while (array[i])
			{
				if (array[i] != NULL)
				{
					free(array[i]);
					array[i++] = NULL;
				}
			}
		}
		free(array);
		array = NULL;
	}
}

void	free_2d(char **array)
{
	if (array == NULL)
		return ;
	if (array[0] == NULL)
	{
		free(array);
		array = NULL;
		return ;
	}
	free_2d_utils(array);
}

void	free_3d_utils(char ***array, int i)
{
	int	j;

	j = 0;
	if (array)
	{
		while (array[i])
		{
			j = 0;
			while (array[i][j])
			{
				if (array[i][j] != NULL)
				{
					free(array[i][j]);
					array[i][j++] = NULL;
				}
			}
			free(array[i]);
			array[i++] = NULL;
		}
		free(array);
		array = NULL;
	}
}

void	free_3d(char ***array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	if (!array[0])
	{
		free(array);
		array = NULL;
		return ;
	}
	if (!array[0][0])
	{
		while (array[i])
			free_2d(array[i++]);
		free(array);
		array = NULL;
		return ;
	}
	free_3d_utils(array, i);
}

void	init_counter(t_counter	*c)
{
	c->i = 0;
	c->j = 0;
	c->k = 0;
	c->m = 0;
	c->n = 0;
}

void	free_everything_utils3(t_shell_s *minishell)
{
	if (minishell->pipes_fd)
		free(minishell->pipes_fd);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell != NULL)
		free(minishell);
}

void	free_everything_utils2(t_shell_s *minishell)
{
	t_counter	c;

	init_counter(&c);
	if (minishell->command_block)
	{
		while (minishell->command_block[c.n])
		{
			if (minishell->command_block[c.n]->command)
				free(minishell->command_block[c.n]->command);
			free_2d(minishell->command_block[c.n]->args);
			if (minishell->command_block[c.n]->files)
			{
				if (minishell->command_block[c.n]->files->redirect_type)
					free(minishell->command_block[c.n]->files->redirect_type);
				free_2d(minishell->command_block[c.n]->files->infile_name);
				free_2d(minishell->command_block[c.n]->files->outfile_name);
				free_2d(minishell->command_block[c.n]->files->append_name);
				free_2d(minishell->command_block[c.n]->files->limiter);
				free(minishell->command_block[c.n]->files);
			}
			free(minishell->command_block[c.n++]);
		}
		free(minishell->command_block);
	}
	free_everything_utils3(minishell);
}

void	free_everything_utils(t_shell_s *minishell)
{
	if (minishell->files)
	{
		free_2d(minishell->files->infile_name);
		free_2d(minishell->files->outfile_name);
		free_2d(minishell->files->append_name);
		free_2d(minishell->files->limiter);
		if (minishell->files->redirect_type)
			free(minishell->files->redirect_type);
		free(minishell->files);
	}
	if (minishell->envp)
	{
		free_2d(minishell->envp->envp);
		free_2d(minishell->envp->key);
		free(minishell->envp->value);
		free_2d(minishell->envp->export_key);
		free_2d(minishell->envp->export_value);
		free_2d(minishell->envp->export_env);
		free(minishell->envp);
	}
	free_everything_utils2(minishell);
}

void	free_everything(t_shell_s *minishell)
{
	if (!minishell)
		return ;
	if (minishell->cmd_line != NULL)
	{
		free(minishell->cmd_line);
		minishell->cmd_line = NULL;
	}
	free_2d(minishell->commands);
	free_2d(minishell->path);
	if (minishell->lexer != NULL)
	{
		free_2d(minishell->lexer->tokens);
		free_2d(minishell->lexer->raw_tokens);
		free_2d(minishell->lexer->command_blocks);
		free(minishell->lexer);
	}
	free_3d(minishell->flags);
	free_everything_utils(minishell);
}

void	free_after_execution_utils2(t_shell_s *minishell)
{
	if (minishell->files)
	{
		free_2d(minishell->files->infile_name);
		free_2d(minishell->files->outfile_name);
		free_2d(minishell->files->append_name);
		free_2d(minishell->files->limiter);
		if (minishell->files->redirect_type)
			free(minishell->files->redirect_type);
		free(minishell->files);
	}
	if (minishell->pipes_fd)
		free(minishell->pipes_fd);
	if (minishell->pid)
		free(minishell->pid);
}

void	free_after_execution_utils(t_shell_s *minishell)
{
	t_counter	c;

	if (minishell->command_block)
	{
		c.i = 0;
		while (minishell->command_block[c.i])
		{
			if (minishell->command_block[c.i]->command)
				free(minishell->command_block[c.i]->command);
			free_2d(minishell->command_block[c.i]->args);
			if (minishell->command_block[c.i]->files)
			{
				free_2d(minishell->command_block[c.i]->files->infile_name);
				free_2d(minishell->command_block[c.i]->files->outfile_name);
				free_2d(minishell->command_block[c.i]->files->append_name);
				free_2d(minishell->command_block[c.i]->files->limiter);
				if (minishell->command_block[c.i]->files->redirect_type)
					free(minishell->command_block[c.i]->files->redirect_type);
				free(minishell->command_block[c.i]->files);
			}
			free(minishell->command_block[c.i++]);
		}
		free(minishell->command_block);
	}
	free_after_execution_utils2(minishell);
}

void	free_after_execution(t_shell_s *minishell)
{
	if (!minishell)
		return ;
	free_3d(minishell->flags);
	free_2d(minishell->commands);
	if (minishell->cmd_line)
	{
		free(minishell->cmd_line);
		minishell->cmd_line = 0;
	}
	if (minishell->lexer)
	{
		free_2d(minishell->lexer->command_blocks);
		free_2d(minishell->lexer->raw_tokens);
		free_2d(minishell->lexer->tokens);
		free(minishell->lexer);
	}
	free_after_execution_utils(minishell);
}

void	free_and_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}
