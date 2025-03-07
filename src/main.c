/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:49:09 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/07 17:59:02 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

// char	*ft_getenv(char **envp, char *var)
// {
// 	int	var_len;
//
// 	if (envp && var)
// 	{
// 		var_len = ft_strlen(var);
// 		while (*envp)
// 		{
// 			if (!ft_strncmp(*envp, var, var_len))
// 				return (*envp + var_len);
// 			envp++;
// 		}
// 	}
// 	return (NULL);
// }

void	*free_vector(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
	return (NULL);
}

// char	**ft_getpath(char *path)
// {
// 	char	**path_list;
// 	char	*tmp;
// 	int		i;
//
// 	path_list = ft_split(path, ':');
// 	if (path_list == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (path_list[i])
// 	{
// 		tmp = ft_strjoin(path_list[i], "/");
// 		if (tmp == NULL)
// 			return (free_vector(path_list));
// 		free(path_list[i]);
// 		path_list[i++] = tmp;
// 	}
// 	return (path_list);
// }

// void	execute_file(char **path_list, char *file, char **av, char **envp)
// {
// 	char	*file_path;
// 	int		i;
//
// 	i = 0;
// 	while (path_list[i])
// 	{
// 		file_path = ft_strjoin(path_list[i++], file);
// 		if (file_path == NULL)
// 			return ;
// 		execve(file_path, av, envp);
// 		free(file_path);
// 	}
// 	perror(file);
// }
//
// int	ft_execvpe(char *file, char **av, char **envp)
// {
// 	char	*path;
// 	char	**path_list;
//
// 	if (ft_strchr(file, '/') && execve(file, av, envp) == ERROR)
// 		return (ERROR);
// 	path = ft_getenv(envp, "PATH=");
// 	if (path == NULL)
// 		return (ERROR);
// 	path_list = ft_getpath(path);
// 	if (path_list == NULL)
// 		return (ERROR);
// 	execute_file(path_list, file, av, envp);
// 	free_vector(path_list);
// 	return (ERROR);
// }

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	exit_program(t_pipe *data, int status)
{
	close_pipe(data->pipe_fd);
	if (data->av[0])
		free_vector(data->av[0]);
	if (data->av[1])
		free_vector(data->av[1]);
	free(data);
	exit(status);
}

void	handle_error(char *str, t_pipe *data)
{
	perror(str);
	exit_program(data, FAILIURE);
}

int	spawn_first_child(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[0], O_RDONLY);
		if (fd == ERROR)
			handle_error(data->file[0], data);
		close(data->pipe_fd[PIPE_RD]);
		dup2(fd, STDIN);
		close(fd);
		dup2(data->pipe_fd[PIPE_WR], STDOUT);
		close(data->pipe_fd[PIPE_WR]);
		ft_execvpe(data->av[0][0], data->av[0], data->envp);
		perror(data->av[0][0]);
		exit_program(data, CMD_NOT_FOUND);
	}
	else if (pid == ERROR)
		return (ERROR);
	return (pid);
}

int	spawn_second_child(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[1], O_WRONLY | O_TRUNC | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == ERROR)
			handle_error(data->file[1], data);
		close(data->pipe_fd[PIPE_WR]);
		dup2(fd, STDOUT);
		close(fd);
		dup2(data->pipe_fd[PIPE_RD], STDIN);
		close(data->pipe_fd[PIPE_RD]);
		ft_execvpe(data->av[1][0], data->av[1], data->envp);
		perror(data->av[1][0]);
		exit_program(data, CMD_NOT_FOUND);
	}
	else if (pid == ERROR)
		return (ERROR);
	return (pid);
}

void	parse_args(char **av, char **envp, t_pipe *data)
{
	data->file[0] = av[1];
	data->file[1] = av[4];
	data->av[0] = ft_tokenize(av[2]);
	data->av[1] = ft_tokenize(av[3]);
	data->envp = envp;
	if (data->av[0] == NULL || data->av[1] == NULL)
		exit_program(data, FAILIURE);
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	*data;
	int		exit_status;

	if (ac != 5)
		return (FAILIURE);
	data = ft_calloc(1, sizeof(t_pipe));
	if (data == NULL)
		return (FAILIURE);
	parse_args(av, envp, data);
	pipe(data->pipe_fd);
	data->pid1 = spawn_first_child(data);
	data->pid2 = spawn_second_child(data);
	close_pipe(data->pipe_fd);
	waitpid(data->pid1, &data->status1, 0);
	waitpid(data->pid2, &data->status2, 0);
	exit_status = SUCCESS;
	if (data->pid1 == ERROR || data->pid2 == ERROR
		|| WEXITSTATUS(data->status2))
		exit_status = FAILIURE;
	if (WEXITSTATUS(data->status2) == CMD_NOT_FOUND)
		exit_status = CMD_NOT_FOUND;
	exit_program(data, exit_status);
}
