/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 13:47:57 by ooutabac          #+#    #+#             */
/*   Updated: 2023/06/07 18:34:34 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <dirent.h>

# define TRUE 0
# define FALSE 1
# define STD_IN 0
# define STD_OUT 1
# define STD_ERROR 2

/* T_COUNTER
- This struct is just for general use to save lines.
It is used so I can easily pass multiple counters between functions and declare
many counters using one variable.
*/
typedef struct s_counter
{
	int	i;
	int	j;
	int	k;
	int	m;
	int	n;
	int	x;
	int	y;
	int	z;
	int	trigger;
	int	trigger2;
	int	counter;
	int	length;
}t_counter;

/* T_LEXER
- This is where tokenization take place. I sepearate everything into tokens
* What is a token?
- A token is any word seperated by space with a few exceptions.
These exceptions include double and single quotes. If a word is joined with double quotes,
the word outside the quotes and the everything within the quotes will be in the same token
regardless of spaces within the quotes. If two quotes are joined together, they will also be
in the same token regardless of space.
* Will I need this struct?
- I assume not, you'll mostly be using the t_shell_s as it contains everything parsed and ready to use.
If anything you need to check, you have the tokens if you had to use it for anything.
*/
typedef struct s_lexer
{
	char	**command_blocks;
	char	**raw_tokens;
	char	**tokens;
	int		num_of_tokens;
	int		num_of_pipes;
	int		num_of_commands;
}t_lexer;

typedef struct s_env_s
{
	int				env_size;
	char			**export_env;
	char			**export_key;
	char			**export_value;
	char			**envp;
	char			**key;
	char			**value;
}t_env_s;

typedef struct s_files
{
	int		num_of_redirections;
	int		infile_fd;
	int		outfile_fd;
	int		append_fd;
	int		heredoc_fd;
	char	**infile_name;
	char	**outfile_name;
	char	**append_name;
	char	**limiter;
	char	*redirect_type;
}t_files;

typedef struct s_execute
{
	int		num_commands;
	int		num_pipes;
	char	*command;
	char	**args;
	char	**tokens;
	t_files	*files;
	t_env_s	*env;
}t_execute;

/* T_SHELL_S
This is the main and most important struct that contains everything

* What is the scary triple pointer char ***flags?
- This is related to char **commands. Flags contain every argument to every command
The reason it is a triple pointer is the way it works. It is quite simple so don't be intimidated
Example:
minishell🤓$ ls -l -la -a | head -n5
Here commands will store "ls" and "head"
commands[0] = "ls"
commands[1] = "head"
Here flags will store the arguments for every command
flags[0][0] = "ls"
flags[0][1] = "-l"
flags[0][2] = "-la"
flags[0][3] = "-a"
flags[1][0] = "head"
flags[1][1] = "-n5"
flags[1][1][0] = '-'
flags[1][1][1] = 'n'
flags[1][1][2] = '5'
and so on

* How do I use flags and why do I need it?
- Flags will be used in the second parameter of the function execve.
execve will take the parameters as such: execve(commands[i], flags[i], envp);
Commands and flags take the same counter. The reason this works is that execve
takes (char *cmd, char **flags, char **envp). So we have an extra pointer on
commands AND flags since we have a list of them.

* What is pipe_fd and why is an int **?
- Pipe_fd is a list of file descriptors. These file descriptors are from the external function pipe()
They are int ** because we have a list of fds because we also have a list of commands
- The number of pipes should always be number_of_commands - 1
The second pointer is the read and write fds that are opened by pipe()
You will be able to write the output of cmd1 into pipe[0][1] and then cmd2 will
read the output of cmd1 as cmd2's input from pipe[0][0]. This is just an example, so
it should work on any number of commands.

* Things I will add later:
- Infile and outfile must be int * since bash accepts more than one redirection of the same type
in the same prompt
*/
typedef struct s_shell_s
{
	int			num_commands;
	int			num_pipes;
	int			*pipes_fd;
	int			heredoc_flag;
	pid_t		*pid;
	int			cmd_used;
	int			exit_code;
	char		***flags;
	char		**commands;
	char		**path;
	char		*cmd_line;
	int			std_in;
	int			std_out;
	t_env_s		*envp;
	t_lexer		*lexer;
	t_files		*files;
	t_execute	**command_block;
}t_shell_s;

/*--------------------------------KHALED-------------------------------*/
/*---------------------------------MAIN--------------------------------*/
void	shell_loop(char **envp, t_shell_s *shell, int i);
void	excute_simple_cmd(t_shell_s *shell);
void	excute_only_redir(t_execute *cmd);
int		forking_required(t_shell_s *shell);
int		exec_child_heredoc(t_shell_s *shell);
void	parent_after_fork(t_shell_s *shell);
void	update_path(t_shell_s *shell);
void	check_if_dir(char *cmd, t_shell_s *shell, int from);
int		path_check(char *cmd, t_shell_s *shell, int from);
void	return_child_notfound(t_shell_s *shell, char *str);
void	excute_child_with_path(t_shell_s *shell, int cmd_num);
void	excute_child_non_builtin(t_shell_s *shell, int cmd_num);
void	excute_child2(t_shell_s *shell, int cmd_num);
void	excute_child(t_shell_s *shell, int cmd_num);
void	minishell_reset(t_shell_s *shell);
void	minishell_init(t_shell_s *shell);
/*---------------------------------pipes--------------------------------*/
void	pid_pipes_init(t_shell_s *shell);
void	pipes_in_child(t_shell_s *shell, int cmd_num);
void	free_error(t_shell_s *shell);
void	close_all_fd(void);
/*---------------------------------redir--------------------------------*/
int		ft_strstrlen(char **str);
int		init_redir(t_execute *cmd);
int		open_exec_heredoc(t_files *files, t_shell_s *shell);
int		open_outfile(t_files *files, int i);
int		open_appendfile(t_files *files, int i);
int		open_infile(t_files *files, int i);
/*--------------------------------signal-------------------------------*/
void	ft_ctrl_c(int sig);
/*--------------------------------builtin-------------------------------*/
int		is_builtin(char *cmd);
int		builtin_exec(t_execute *exec, t_shell_s *shell);
int		ft_echo(char **args);
int		ft_env(char **arg, t_env_s *env);
int		ft_pwd(void);
int		unset_args_check(char *str);
void	remove_env_from_list(t_env_s *env, int position);
void	remove_export_from_list(t_env_s *env, int position);
int		unset_arg_comp(char *arg, char **env_key);
int		export_arg_comp(char *arg, char **env_key);
int		ft_unset(t_execute *exec);
int		get_pwd_p_env(t_env_s *env);
int		get_pwd_p_export(t_env_s *env);
void	add_to_env(t_env_s *env, char *pwd, char *old_pwd);
int		path_env_check(t_execute *cmd, char **path);
int		ft_cd(t_execute *cmd);
void	get_flag(int *flag, char *str);
void	add_only_key(t_env_s *env, char *str);
void	add_args_export(t_env_s *env, int *flag, char *str);
void	add_args_env(t_env_s *env, int *flag, char *str);
void	add_export_args(char *str, int *flag, t_env_s *env);
void	swap_key_value(char **key, char **value, int i);
void	sort_key_value(char **key, char **value);
void	env_export_printing(t_env_s *env);
int		export_args_check(char *str);
int		check_in_export(t_env_s *env, char *str, int *flag);
void	add_to_export(t_env_s *env, char *pwd_export, char *old_pwd);
int		check_in_env(t_env_s *env, char *str, int *flag);
void	add_value_to_env(int p, t_env_s *env, char *str, int *flag);
void	add_value_to_export(int p, t_env_s *env, char *str, int *flag);
void	ft_export2(t_execute *cmd, int *flag, int p, int i);
void	exit_export(char *str);
void	export_init(t_shell_s *shell, int flag);
int		ft_export(t_execute *cmd);
void	ft_exit(t_execute *exec, t_shell_s *shell);

/*----------------------------------OBADA-----------------------------------*/
/*---------------------------------PARSING----------------------------------*/
t_shell_s	*parse(t_shell_s *minishell, char *str, char **envp, int iteration);

/*-----------------------------------SKIP-----------------------------------*/
int			skip_spaces(char *str, int i);
int			skip_token(char *str, int i);
int			skip_symbols(char *str, int i);
int			skip_squotes(char *str, int i);
int			skip_dquotes(char *str, int i);

/*----------------------------COUNT_REDIRECTIONS----------------------------*/
int			count_infiles(char	**str);
int			count_outfiles(char	**str);
int			count_appends(char	**str);
int			count_heredocs(char	**str);

/*----------------------------------UTSIL1----------------------------------*/
int			ft_strlen_spaces(char *str, int i);
int			ft_strlen_equals(char *str);
int			count_tokens(char *str);
int			count_pipes(char *str);

/*----------------------------------UTSIL2----------------------------------*/
t_shell_s	*get_path(t_shell_s	*minishell, char **envp);
t_shell_s	*get_env_struct(t_shell_s *minishell, char *envp[]);
t_shell_s	*get_flags(t_shell_s *minishell);
t_shell_s	*get_commands(t_shell_s *minishell);
t_shell_s	*get_num_commands(t_shell_s *minishell);
t_shell_s	*get_execution_blocks(t_shell_s *minishell);
t_execute	*get_files(t_shell_s *minishell, t_execute *execute_block,
				char **command_block, char **raw_command_block);

/*----------------------------------UTSIL3----------------------------------*/
t_shell_s	*lexer(t_shell_s *minishell, char *str);
int			number_of_dquotes(char *str);
int			number_of_squotes(char *str);
t_shell_s	*malloc_lexer(t_shell_s *minishell, char *str);
void		lexer_double_symbol(t_shell_s *minishell,
				t_counter *c, char *str);
void		lexer_single_symbol(t_shell_s *minishell,
				t_counter *c, char *str);
char		**skip_double_quote_and_assign(
				t_counter *count, char *str, char **blocks);
char		**skip_single_quote_and_assign(
				t_counter *count, char *str, char **blocks);

/*----------------------------------UTSIL4----------------------------------*/
int			num_of_tokens(char *str);
int			token_size(char *str, int i);
int			is_heredoc_append(char *str, t_counter *c);
int			is_infile_outfile_pipe(char *str, t_counter *c);

/*----------------------------------UTSIL5----------------------------------*/
int			check_if_command(t_shell_s *minishell, char *token, int token_num);
int			get_num_flags(char **token, int i);
int			check_validity(t_shell_s *minishell, char *str);

/*----------------------------------UTSIL6----------------------------------*/
t_shell_s	*raw_lexer(t_shell_s *minishell, char *str);
int			raw_token_size(char *str, int i);
char		**split_pipes(char	*str);
char		**split_command_block(t_shell_s *minishell, int token_num);
char		**split_raw_command_block(t_shell_s *minishell, int token_num);
int			num_of_tokens_to_pipe(char **tokens, int token_num);
int			length_to_pipe(char *str, int i);

/*--------------------------------DOLLAR_SIGN1------------------------------*/
char		*dollar_sign(t_shell_s *minishell, char *cmd_line);

/*------------------------------------FREE----------------------------------*/
void		free_everything(t_shell_s *minishell);
void		free_after_execution(t_shell_s *minishell);
void		free_and_null(void *ptr);
void		free_2d(char **array);
void		free_3d(char ***array);

#endif