/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excution.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 20:07:28 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/24 10:35:29 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	parent_after_fork(t_shell_s *shell)
{
	int	i;

	i = 0;
	if (shell->num_pipes > 0)
	{
		while (i < shell->num_pipes * 2)
			close(shell->pipes_fd[i++]);
	}
	i = -1;
	while (++i < shell->num_commands)
		waitpid(shell->pid[i], NULL, WUNTRACED);
	dup2(shell->std_out, STDOUT_FILENO);
	dup2(shell->std_in, STDIN_FILENO);
}

void	exec_child_heredoc(t_shell_s *shell)
{
	int i;

	i = -1;
	while (++i < shell->num_commands)
	{
		shell->command_block[i]->excuted = 0;
		if (shell->command_block[i]->files->limiter[0] != NULL)
		{
			// shell->command_block[i]->excuted = 1;
			init_heredoc(shell->command_block[i], shell);
			// excute_child(shell, i);
		}
	}
}

/*
check if forking required in order to call fork or excute by parent
*/
static int	forking_required(t_shell_s *shell)
{
	if (shell->num_pipes == 0 && is_builtin(shell->command_block[0]->command))
		return (1);
	return (0);
}

/*
it have 2 roles, 
1- it check if forking required and if not it will excute the builtin by 
	make the redirection
	excute the builtin
	return any redir to original
2- it will loop and fork the proccess in each loop to execute the child
*/
static void	start_exec(t_shell_s *shell)
{
	shell->cmd_used = -1; // to add it to parsing
	shell->std_in = dup(STDIN_FILENO); // to add it to parsing
	shell->std_out = dup(STDOUT_FILENO); // to add it to parsing
	// check sytax error
	if (forking_required(shell))
	{
		if (init_redir(shell->command_block[0], shell) == -1)
		{
			shell->exit_code = EXIT_FAILURE;
			return ;
		}
		shell->exit_code = builtin_exec(shell->command_block[0]);
	}
	else
	{
		pid_pipes_init(shell);
		exec_child_heredoc(shell);
		while (++shell->cmd_used < shell->num_commands)
		{
			if (shell->command_block[shell->cmd_used]->excuted == 0)
			{
				shell->command_block[shell->cmd_used]->excuted = 1;
				excute_child(shell, shell->cmd_used);
			}
		}
		parent_after_fork(shell);
	}
	//return (status);
}

/*
chech if it reads spaces by all types
*/
static int	check_cmd(char *cmd)
{
	int		i;

	i = -1;
	
	while (cmd[++i])
		if (cmd[i] > 32 )
			return (0);
	return (1);
}

/*
this the core function that have infinit loop the keep read the input and 
then do parsing and if no errors it will start excution
*/
int	shell_loop(char **envp)
{
	t_shell_s	*shell;
	char		*cmd;

	while (1)
	{
		cmd = readline("minishell🤓$ ");
		if (!cmd)
			break ;
		if (!cmd || check_cmd(cmd))
			continue ;
		add_history(cmd);
		shell = parse(cmd, envp);
		free(cmd);
		cmd = NULL;
		if (shell)
		{
			start_exec(shell);
			free_everything(shell);
		}
	}
	return (0);
}
