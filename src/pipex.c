/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:32:40 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 08:06:52 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	spawn_first_child(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[FILE_IN], O_RDONLY);
		if (fd == ERROR)
			handle_error(FAILIURE, data->file[FILE_IN], data);
		close(data->pipe_fd[PIPE_RD]);
		dup2(fd, STDIN);
		close(fd);
		dup2(data->pipe_fd[PIPE_WR], STDOUT);
		close(data->pipe_fd[PIPE_WR]);
		ft_execvpe(data->av[0][0], data->av[0], data->envp);
		handle_error(CMD_NOT_FOUND, data->av[0][0], data);
	}
	else if (pid == ERROR)
		handle_error(FAILIURE, "fork", data);
}

pid_t	spawn_last_child(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[FILE_OUT], O_WRONLY | O_TRUNC | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == ERROR)
			handle_error(FAILIURE, data->file[FILE_OUT], data);
		close(data->pipe_fd[PIPE_WR]);
		dup2(fd, STDOUT);
		close(fd);
		dup2(data->pipe_fd[PIPE_RD], STDIN);
		close(data->pipe_fd[PIPE_RD]);
		ft_execvpe(data->av[1][0], data->av[1], data->envp);
		handle_error(CMD_NOT_FOUND, data->av[1][0], data);
	}
	else if (pid == ERROR)
		handle_error(FAILIURE, "fork", data);
	return (pid);
}
