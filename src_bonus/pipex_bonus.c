/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:32:40 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 08:06:52 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdio.h>

pid_t	spawn_first_child(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	if (pipe(data->pipe_fd) == ERROR)
		handle_error(FAILIURE, "pipe", data);
	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[0], O_RDONLY);
		if (fd == ERROR)
			handle_error(FAILIURE, data->file[0], data);
		close(data->pipe_fd[PIPE_RD]);
		dup2(fd, STDIN);
		close(fd);
		dup2(data->pipe_fd[PIPE_WR], STDOUT);
		close(data->pipe_fd[PIPE_WR]);
		ft_execvpe(data->av[0][0], data->av[0], data->envp);
		handle_error(CMD_NOT_FOUND, data->av[0][0], data);
	}
	else if (pid == ERROR)
		return (ERROR);
	return (pid);
}

void	spawn_middle_children(t_pipe *data)
{
	int	i;

	i = 1;
	while (i < data->cmd_count - 1)
	{
		close(data->pipe_fd[PIPE_WR]);
		dup2(data->pipe_fd[PIPE_RD], STDIN);
		close(data->pipe_fd[PIPE_RD]);
		pipe(data->pipe_fd);
		if (fork() == 0)
		{
			close(data->pipe_fd[PIPE_RD]);
			printf("n:	%s %s\n", data->av[i][0], data->av[i][1]);
			dup2(data->pipe_fd[PIPE_WR], STDOUT);
			close(data->pipe_fd[PIPE_WR]);
			ft_execvpe(data->av[i][0], data->av[i], data->envp);
			handle_error(CMD_NOT_FOUND, data->av[i][0], data);
		}
		i++;
	}
}

pid_t	spawn_last_child(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[1], O_WRONLY | O_TRUNC | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == ERROR)
			handle_error(FAILIURE, data->file[1], data);
		close(data->pipe_fd[PIPE_WR]);
		dup2(fd, STDOUT);
		close(fd);
		dup2(data->pipe_fd[PIPE_RD], STDIN);
		close(data->pipe_fd[PIPE_RD]);
		ft_execvpe(data->av[data->cmd_count - 1][0], data->av[data->cmd_count - 1],
			data->envp);
		handle_error(CMD_NOT_FOUND, data->av[data->cmd_count - 1][0], data);
	}
	else if (pid == ERROR)
		return (ERROR);
	return (pid);
}
