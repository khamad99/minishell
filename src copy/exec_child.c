/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 22:18:42 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/21 10:59:25 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
F_OK: This constant is used to check if the file or directory exists.
X_OK: This constant is used to check if the file or directory is executable.
*/
int	path_check(char *cmd)
{
	if (!access(cmd, F_OK))
	{
		if (!access(cmd, X_OK))
			return (1);
		else
			//access_denied
			return (0);
	}
	else
		return (0);
}

/*
this function excute the non_builtin command, it do the following
1- update the env
2- check if bath provided and excute if yes
3- if no join the path with cmd
	check the validity by access function
	yes -> execve
	no -> error - not found 
*/

void	excute_child_non_builtin(t_shell_s *shell, int cmd_num)
{
	int	i;
	char	*cmd_with_path;
	// check env
	if (path_check(shell->command_block[cmd_num]->command))
		execve(shell->command_block[cmd_num]->command,
			shell->command_block[cmd_num]->args, shell->envp->envp);
	i = -1;
	while (shell->path[++i])
	{
		cmd_with_path = ft_strjoin(shell->path[i], "/");
		cmd_with_path = ft_strjoin(cmd_with_path, shell->command_block[cmd_num]->command);
		if (path_check(cmd_with_path))
			execve(cmd_with_path, shell->command_block[cmd_num]->args, shell->envp->envp);
		free(cmd_with_path);
	}
	
	// no -> error - not found 

}

/*
this function will be caled if forking is requied according the test
it will do the following:
1- fork the process
2- handle the pipes in child proces
3- apply the redir 
4- excute builin if it is so
5- call the function that excute the non_builtin 
6- wait for child to finish
*/
void	excute_child(t_shell_s *shell, int cmd_num)
{
	int	status;

	status = 0;
	shell->pid[cmd_num] = fork();
	if (shell->pid[cmd_num] == -1)
		return ; // fix the free function later
	else if (shell->pid[cmd_num] == 0)
	{
		if (shell->num_pipes > 0)
			pipes_in_child(shell, cmd_num);
		if (init_redir(shell->command_block[cmd_num]) == -1)
			return ; // to handle the no_acess to infile
		if (is_builtin(shell->command_block[cmd_num]->command))
		{
			status = builtin_exec(shell->command_block[0]);
			// free chiild process
			exit(status);
		}
		else
			excute_child_non_builtin(shell, cmd_num);
	}
	else if (shell->pid[cmd_num] > 0)
		waitpid(shell->pid[cmd_num], NULL, WUNTRACED);
}