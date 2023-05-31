/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:26 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/31 07:24:13 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int g_exit_code;

/*
Function Purpose:
The "exit" function allows the user to quit the program at any given time by calling it.

Handling Different Scenarios:

If the "exit" function is called without any arguments, it will return the exit status of the last executed function or program.
If the "exit" function receives more than one argument, it will print an error message and return 2.
If the "exit" function receives a non-numeric argument, it will print an error message and return 2.
Numeric Argument Handling:

If the "exit" function receives a numeric argument:
If the argument is less than 256, it will be returned as the exit status.
If the argument is greater than 0, the function will return 256 minus the unsigned value of the argument. This calculation is done in base 256.
If the argument is greater than 256, the function will return the argument modulo 256 (% arg).
Specific Exit Status Numbers:
The statement mentions specific exit status numbers and their meanings, which are commonly used in Bash scripts. These numbers include:
1: Catchall for general errors.
2: Misuse of shell built-ins.
126: Command invoked cannot execute.
127: Command not found.
128: Invalid argument to the exit command.
128+n: Fatal error signal "n".
130: Bash script terminated by Control-C.
255*: Exit status out of range.

It's important to note that the specific exit status numbers mentioned in the code may follow a convention or are commonly used in Bash scripts, but their usage and interpretation can vary across different programming languages or systems.

Overall, the "exit" function provides a way to terminate a program and return an exit status, which can be used to convey information about the program's execution or any encountered errors.


1: Catchall for general errors:
Exit status 1 is a general catchall for indicating that an error occurred during the execution of a program. It is commonly used to represent unexpected or unspecified errors that do not have a more specific exit status associated with them.

2: Misuse of shell built-ins:
Exit status 2 is typically used to indicate a misuse or incorrect usage of shell built-in commands. It signifies that the user has used a shell built-in command in an improper or unsupported way, leading to an error.

126: Command invoked cannot execute:
Exit status 126 is used when a command or executable is invoked but cannot be executed. It typically indicates that the permission settings or execution rights for the command are not properly configured, preventing its execution.

127: Command not found:
Exit status 127 is used when a command is not found or not recognized by the shell. It usually indicates that the command entered by the user does not exist or is not accessible in the current environment.

128: Invalid argument to the exit command:
Exit status 128 is specifically associated with the "exit" command itself. It is used when an invalid or unsupported argument is provided to the "exit" command. This can happen if a non-numeric argument is provided or if the argument is outside the allowed range of exit status values.

128+n: Fatal error signal "n":
Exit statuses in the range of 128+n are used to indicate fatal error signals. When a program receives a fatal error signal, such as a termination signal from the operating system, the exit status will be 128 plus the signal number (n). For example, if a program receives a termination signal 9 (SIGKILL), the exit status will be 128+9=137.

130: Bash script terminated by Control-C:
Exit status 130 is used when a Bash script is terminated by the user pressing Control-C (SIGINT signal). It indicates that the script execution was interrupted by the user's action.

255*: Exit status out of range:
Exit status 255 is typically used to indicate that the exit status value is out of the valid range. This can occur if the exit status is explicitly set to a value greater than 255, which is the maximum allowed exit status value.

These exit status numbers provide a way to convey specific information about the execution of a program or script. By examining the exit status, users or other programs can determine the outcome of the execution and take appropriate actions or make decisions based on that information.

*/

/*
for arg it should be numaric between ULLi 9223372036854775807 and -9223372036854775808
++ or -- not allowed (only one)

*/

static	unsigned long long int 	ft_my_attoi(char *str, int *s)
{
	unsigned long long	r;
	char				*tmp;

	tmp = str;
	*s = 1;
	r = 0;
	if (*str == '-')
		*s = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (ft_isdigit(*str))
	{
		r = r * 10 + (*str - '0');
		str++;
	}
	if (*s == -1 && r == 9223372036854775807)
		return (2);
	if (!ft_strncmp("-9223372036854775808", tmp, 20))
		return (3);
	if (*s == -1 && r > 9223372036854775807)
		return (1);
	if (*s == 1 && r > 9223372036854775807)
		return (1);
	return (r);
}

static void	exit_ok(unsigned long long int	num, int *s, t_shell_s *shell)
{
	int	r;
 
	r = 0;
	if (*s == 1 && num >= 256)
		r = num % 256;
	else if (*s == -1)
	{
		if (num > 256)
			r = num % 256;
		else
			r = 256 - num;
	}
	else
		r = num;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	g_exit_code = r;
	close_all_fd();
	free_everything(shell);
	exit(g_exit_code);
}


static int	check_exit_args(char *str, int *s)
{
	int	i;

	i = 0;
	if (!str[i] || (!ft_isdigit(str[i]) && str[i] != '+' && str[i] != '-'))
		return (1);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) != 1 || i > 20)
			return (1);
		i++;
	}
	if (ft_my_attoi(str, s) == 1 && i > 3)
		return (1);
	if (ft_my_attoi(str, s) == 2 && i > 3)
		return (2);
	if (ft_my_attoi(str, s) == 3 && i > 3)
		return (3);
	return (0);
}

void	ft_exit(t_execute *exec, t_shell_s *shell)
{
	int	s;

	if (!exec->args[1])
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		close_all_fd();
		free_everything(shell);
		exit(g_exit_code); // with last exit code 
	}
	else if (exec->args[2])
	{
		ft_putstr_fd("exit\nminishell: exit: too many arguments\n", STDERR_FILENO);
		close_all_fd();
		free_everything(shell);
		exit(EXIT_FAILURE);
	}
	else if (exec->args[1] && !check_exit_args(exec->args[1], &s))
		exit_ok(ft_my_attoi(exec->args[1], &s), &s, shell);
	else if (exec->args[1] && check_exit_args(exec->args[1], &s) == 2)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		close_all_fd();
		free_everything(shell);
		exit(EXIT_FAILURE);
	}
	else if (exec->args[1] && check_exit_args(exec->args[1], &s) == 3)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		close_all_fd();
		free_everything(shell);
		exit(EXIT_SUCCESS);
	}
	else
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(exec->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		close_all_fd();
		free_everything(shell);
		exit(2);
	}
	
}