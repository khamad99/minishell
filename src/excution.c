/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excution.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 20:07:28 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/14 00:24:48 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
static int	start_exec(t_shell_s *shell)
{
	shell->cmd_used = 0;
	if (forking_required(shell) && is_builtin(shell->command_block[0]->command))
	{
		// make the redirection
		shell->exit_code = builtin_exec(shell->command_block[0]);
		// return any redir to original
		//dup2();
		//dup2();
	}
	else
	{
		pipes_pid_init(shell);
		while (shell->cmd_used < shell->num_commands)
			excute_child(shell, shell->cmd_used++);
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
		if (!cmd || check_cmd(cmd))
			continue ;
		add_history(cmd);
		shell = parse(cmd, envp);
		free(cmd);
		cmd = NULL;
		if (shell)
		{
			// int i = -1;
			// while ( ++i < shell->num_commands && shell)
			// 	printf("command->%s\nARGS->%s\n", shell->command_block[i]->command, shell->command_block[i]->args[1]);
			// printf("%d\n", shell->command_block[0]->num_pipes );
			//printf("redir num -> %d\nif->%d\nof->%d\naf->%d\nhf->%d\ntype->%s\n", shell->files->num_of_redirections, shell->files->infile_fd, shell->files->outfile_fd, shell->files->append_fd, shell->files->heredoc_fd, shell->files->redirect_type);
			start_exec(shell);
			free_everything(shell);
		}
	}
	return (0);
}