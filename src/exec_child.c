/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 22:18:42 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/30 23:02:18 by kalshaer         ###   ########.fr       */
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
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(" : Permission denied\n", STDERR_FILENO);
			return (-1);
		}
	}
	else
		return (0);
}



// static void	control_parent_after_fork(t_shell_s *shell, int cmd_num)
// {
// 	int status;
// 	if (cmd_num == shell->num_pipes && shell->num_pipes > 0 && cmd_num % 2 == 0)
// 	{
// 		close(shell->pipes_fd[0]);
// 		close(shell->pipes_fd[1]);
// 		//close(shell->pipes_fd[2]);
// 		close(shell->pipes_fd[3]);
// 	}
// 	else if (cmd_num == shell->num_pipes && shell->num_pipes > 0 && cmd_num % 2 != 0)
// 	{
// 		//close(shell->pipes_fd[0]);
// 		close(shell->pipes_fd[1]);
// 		close(shell->pipes_fd[2]);
// 		close(shell->pipes_fd[3]);
// 	}
// 	while (1) 
// 	{
// 		int result = waitpid(shell->pid[cmd_num], &status, WNOHANG);
// 		if (result == 0) 
// 		{
// 			// Child process has not changed state yet
// 			printf("Child process is still running...\n");
// 			sleep(2);
// 		} else {
// 			// Child process has changed state
// 			if (WIFEXITED(status)) {
// 				printf("Child exited with status: %d\n", WEXITSTATUS(status));
// 			}
// 			break;
// 		}
// 	}

// // 	waitpid(shell->pid[cmd_num], &status, WUNTRACED);
// // 	if (WIFEXITED(status)) 
// //         printf("Child exited with status: %d\n", WEXITSTATUS(status));
// // 	if (cmd_num == shell->num_pipes && shell->num_pipes > 0 && cmd_num % 2 == 0)
// // 		close(shell->pipes_fd[2]);
// // 	else if (cmd_num == shell->num_pipes && shell->num_pipes > 0 && cmd_num % 2 != 0)
// // 		close(shell->pipes_fd[0]);
// }

/*
this function excute the non_builtin command, it do the following
1- update the env
2- check if bath provided and excute if yes
3- if no join the path with cmd
	check the validity by access function
	yes -> execve
	no -> error - not found 
*/
static void	excute_child_non_builtin(t_shell_s *shell, int cmd_num)
{
	int	i;

	char	*cmd_with_path;
	// char *arg[] = {"./test1", NULL};
	// // check if env updated and update the paths 
	// execve("./test1", arg, shell->envp->envp);
	if (path_check(shell->command_block[cmd_num]->command) == 1)
		execve(shell->command_block[cmd_num]->command,
			shell->command_block[cmd_num]->args, shell->envp->envp);
	else if (path_check(shell->command_block[cmd_num]->command) == -1)
	{
		free_error(shell);
		exit(126);
	}
	//perror("execve");
	if (!shell->path)
	{
		ft_putstr_fd("minishell: Command not found: ", STDERR_FILENO);
		ft_putstr_fd(shell->command_block[cmd_num]->command, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free_error(shell);
		exit(127);
	}
	i = -1;
	while (shell->path[++i])
	{
		cmd_with_path = ft_strjoin(shell->path[i], shell->command_block[cmd_num]->command);
		if (path_check(cmd_with_path))
			execve(cmd_with_path, shell->command_block[cmd_num]->args, shell->envp->envp);
		free(cmd_with_path);
	}
	ft_putstr_fd("minishell: Command not found: ", STDERR_FILENO);
	ft_putstr_fd(shell->command_block[cmd_num]->command, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free_error(shell);
	exit(127);
}

// void	close_heredok_pipes(t_shell_s *shell, int cmd_num)
// {
// 	int	i;

// 	if (shell->num_pipes <= 0)
// 		return ;
// 	i = -1;
// 	while (++i < shell->num_pipes * 2)
// 		if (((cmd_num * 2) - 2) != i && ((cmd_num * 2) + 1) != i)
// 			close(shell->pipes_fd[i]);
// 	if (cmd_num == 0)
// 	{
// 		close(shell->pipes_fd[1]);
// 	}
// 	else if (cmd_num == shell->num_pipes)
// 	{
// 		close(shell->pipes_fd[(cmd_num * 2) - 2]);
// 	}
// 	else
// 	{
// 		close(shell->pipes_fd[(cmd_num * 2) - 2]);
// 		close(shell->pipes_fd[(cmd_num * 2) + 1]);
// 	}
// }

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
	{
		ft_putstr_fd("fork failure\n", STDERR_FILENO);
		free_error(shell);
	}
	else if (shell->pid[cmd_num] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		pipes_in_child(shell, cmd_num);
		if (init_redir(shell->command_block[cmd_num], shell) == -1)
		{
			free_error(shell);
			exit(EXIT_FAILURE);
		}
		if (shell->command_block[cmd_num]->command)
		{
			if (is_builtin(shell->command_block[cmd_num]->command))
			{
				status = builtin_exec(shell->command_block[cmd_num], shell);
				free_error(shell);
				exit(status);
			}
			else
				excute_child_non_builtin(shell, cmd_num);
		}
		free_error(shell);
		exit(EXIT_SUCCESS);
	}
}
