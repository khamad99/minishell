#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	struct sigaction	s_quit;
	struct sigaction	s_int;
	
	(void)argc;
	(void)argv;
	(void)envp;
	s_quit.sa_handler = SIG_IGN;
	s_int.sa_sigaction = ft_ctrl_c;
	s_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &s_int, NULL) == -1
		|| sigaction(SIGQUIT, &s_quit, NULL) == -1)
	{
		ft_putstr_fd("Error: Failed sigaction\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
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


	