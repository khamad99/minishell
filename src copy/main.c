#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	signal(SIGINT, ft_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(envp);

	return (0);
}



	// av = ft_split(cmd, ' ');
	// if(!cmd)
	// 	exit(EXIT_SUCCESS);
	// if (ft_strlen(cmd) > 0)
	// 	add_history(cmd);
	// if(cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
	// {
	// 	free(cmd);
	// 	return (1);
	// }
	// if(ft_strncmp(cmd, "exit ", 5) == 0)
	// {
	// 	free(cmd);
	// 	return (2);
	// }
	// cmd = ft_strjoin("/bin/", av[0]);
	// if (access(cmd, 0) == 0)
	// {
	// 	*flag = 1;
	// }
	// if(*flag)
	// {
	// 	pid = fork();
	// 	if(pid == 0)
	// 		execve(cmd, av, envp);
	// }


	