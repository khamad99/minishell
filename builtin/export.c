/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/06/01 15:03:55 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



void add_export_args(char *str, int *flag, t_env_s *env)
{
	char 	**temp_envp;
	char 	**temp_key;
	char 	**temp_value;
	char 	**temp_envp_ex;
	char 	**temp_key_ex;
	char 	**temp_value_ex;
	int		i;

	i = -1;
	while (env->export_key[++i])
		if (!ft_strncmp(str, env->export_key[i], ft_strlen(str)))
			return ;
	temp_envp_ex = ft_calloc(ft_strstrlen(env->export_env) + 2, sizeof(char *));
	temp_key_ex = ft_calloc(ft_strstrlen(env->export_key) + 2, sizeof(char *));
	temp_value_ex = ft_calloc(ft_strstrlen(env->export_value) + 2, sizeof(char *));
	i = -1;
	if (*flag != 0)
	{
		temp_envp = ft_calloc(ft_strstrlen(env->envp) + 2, sizeof(char *));
		temp_key = ft_calloc(ft_strstrlen(env->key) + 2, sizeof(char *));
		temp_value = ft_calloc(ft_strstrlen(env->value) + 2, sizeof(char *));
		while (env->envp[++i])
		{
			temp_envp[i] = ft_strdup(env->envp[i]);
			temp_key[i] = ft_strdup(env->key[i]);
			temp_value[i] = ft_strdup(env->value[i]);
		}
		temp_envp[i] = ft_strdup(str);
		temp_key[i] = ft_calloc(ft_strlen(str) - *flag + 1, sizeof(char));
		ft_strlcpy(temp_key[i], str, *flag + 1);
		temp_value[i] = ft_strdup(str + *flag + 1);
		temp_envp[i + 1] = 0;
		temp_key[i + 1] = 0;
		temp_value[i + 1] = 0;
		i = -1;
		while (env->export_env[++i])
		{
			temp_envp_ex[i] = ft_strdup(env->export_env[i]);
			temp_key_ex[i] = ft_strdup(env->export_key[i]);
			temp_value_ex[i] = ft_strdup(env->export_value[i]);
		}
		temp_envp_ex[i] = ft_strdup(str);
		temp_key_ex[i] = ft_calloc(ft_strlen(str) - *flag + 1, sizeof(char));
		ft_strlcpy(temp_key_ex[i], str, *flag + 1);
		temp_value_ex[i] = ft_strdup(str + *flag + 1);
		temp_envp_ex[i + 1] = 0;
		temp_key_ex[i + 1] = 0;
		temp_value_ex[i + 1] = 0;
		// free_2d(env->envp);
		// free_2d(env->key);
		// free_2d(env->value);
		// free_2d(env->export_env);
		// free_2d(env->export_key);
		// free_2d(env->export_value);
		env->envp = temp_envp;
		env->key = temp_key;
		env->value = temp_value;
		env->export_env = temp_envp_ex;
		env->export_key = temp_key_ex;
		env->export_value = temp_value_ex;
	}
	else
	{
		while (env->export_value[++i])
			temp_value_ex[i] = ft_strdup(env->export_value[i]);
		i = -1;
		while (env->export_env[++i])
		{
			temp_envp_ex[i] = ft_strdup(env->export_env[i]);
			temp_key_ex[i] = ft_strdup(env->export_key[i]);
		}
		temp_envp_ex[i] = ft_strdup(str);
		temp_key_ex[i] = ft_strdup(str);
		temp_value_ex[i] = NULL;
		temp_envp_ex[i + 1] = 0;
		temp_key_ex[i + 1] = 0;
		temp_value_ex[i + 1] = 0;
		free_2d(env->export_env);
		free_2d(env->export_key);
		free_2d(env->export_value);
		env->export_env = temp_envp_ex;
		env->export_key = temp_key_ex;
		env->export_value = temp_value_ex;
		
	}
	
		
}

static void sort_env(char **envp) 
{
	int		i;
	char	*temp;
	int		len;
	int		j;

	j = -1;
	len = ft_strstrlen(envp);
	while (++j < len)
	{
		i = -1;
		while (envp[++i])
		{
			if (envp[i + 1] && ft_strncmp(envp[i], envp[i + 1], ft_strlen(envp[i])) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
			}
		}
	}
}

static void sort_key_value(char **key, char **value) 
{
	int		i;
	char	*temp;
	char	*temp2;
	int		len;
	int		j;

	j = -1;
	len = ft_strstrlen(key);
	while (++j < len)
	{
		i = -1;
		while (key[++i])
		{
			if (key[i + 1] && ft_strncmp(key[i], key[i + 1], ft_strlen(key[i])) > 0)
			{
				temp = key[i];
				temp2 = value[i];
				key[i] = key[i + 1];
				value[i] = value[i + 1];
				key[i + 1] = temp;
				value[i + 1] = temp2;
			}
		}
	}
}

static void	env_export_printing(t_env_s *env)
{
	int	i;

	sort_env(env->export_env);
	sort_key_value(env->export_key, env->export_value);
	i = -1;
	while (env->export_env[++i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->export_key[i], STDOUT_FILENO);
		if (env->export_value[i])
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(env->export_value[i], STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

/*
1 -> do nothing
0 -> not valid
2 -> ok to add
*/

// {
// 	!;
// 	@;
// 	$;
// 	%;
// 	^;
// 	-;
// 	" ";
// 	"\";
// 	_; //->one only will do nothing if it is alone
// }
static int	export_args_check(char *str, int *flag)
{
	int		i;
	char	r[10];

	ft_strlcpy(r, " !@$%^-\\", 10);
	i = -1;
	*flag = 0;
	if (!ft_strncmp(str, "_\0", 2))
		return (1);
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
			*flag = i;
		else if (*flag == 0 && ft_strchr(r, str[i]) != NULL)
			return (0);
		else if (!str[i + 1])
			return (1);
	}
	return (0);
}

static int	check_in_export(t_env_s *env, char *str, int *flag)
{
	int		i;
	char	*key;
	
	i = -1;
	*flag = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
		{
			*flag = i;
			break ;
		}
	}
	if (*flag != 0)
	{
		key = ft_calloc(*flag + 1, sizeof(char));
		ft_strlcpy(key, str, *flag);
		//key[*flag + 1] = 0;
		i = -1;
		while (env->export_key[++i])
		{
			if (!ft_strncmp(key, env->export_key[i], ft_strlen(key)))
			{
				free(key);
				return (i);
			}
		}
		free(key);
		return (-1);
	}
	return (-1);
}

static int	check_in_env(t_env_s *env, char *str, int *flag)
{
	int		i;
	char	*key;
	
	i = -1;
	*flag = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
		{
			*flag = i;
			break ;
		}
	}
	if (*flag != 0)
	{
		key = ft_calloc(*flag + 1, sizeof(char));
		ft_strlcpy(key, str, *flag);
		i = -1;
		while (env->key[++i])
		{
			if (!ft_strncmp(key, env->key[i], ft_strlen(key)))
			{
				free(key);
				return (i);
			}
		}
		free(key);
		return (-1);
	}
	return (-1);
}

void	add_value_to_env(int p, t_env_s *env, char *str, int *flag)
{
	(void)flag;
	free(env->envp[p]);
	env->envp[p] = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	ft_strlcpy(env->envp[p], str, ft_strlen(str) + 1);
	// free(env->value[p]);
	// env->value[p] = ft_calloc(ft_strlen(str) - *flag + 1, sizeof(char));
	// ft_strlcpy(env->value[p], str + *flag + 1, ft_strlen(str) - *flag);
}

void	add_value_to_export(int p, t_env_s *env, char *str, int *flag)
{
	free(env->export_env[p]);
	env->export_env[p] = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	ft_strlcpy(env->export_env[p], str, ft_strlen(str));
	free(env->export_value[p]);
	env->export_value[p] = ft_calloc(ft_strlen(str) - *flag + 1, sizeof(char));
	ft_strlcpy(env->export_value[p], str + *flag + 1, ft_strlen(str) - *flag);
}

int	ft_export(t_execute *cmd)
{	
	int		i;
	int		p;
	int		flag;
	int		exit_code;

	i = 0;
	exit_code = 0;
	if (!cmd->args[1])
	{
		env_export_printing(cmd->env);
		return (0);
	}
	else
	{
		while (cmd->args[++i])
		{
			p = check_in_export(cmd->env, cmd->args[i], &flag);
			if (p != -1)
			{
				add_value_to_export(p, cmd->env, cmd->args[i], &flag);
				p = check_in_env(cmd->env, cmd->args[i], &flag);
				if (p != -1)
					add_value_to_env(p, cmd->env, cmd->args[i], &flag);
			}
			else if (export_args_check(cmd->args[i], &flag))
				add_export_args(cmd->args[i], &flag, cmd->env);
			else
			{
				ft_putstr_fd("minishell: export: ", STDERR_FILENO);
				ft_putstr_fd(cmd->args[i], STDERR_FILENO);
				ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
				exit_code = 1;
			}
		}
	}
	return (exit_code);
}
