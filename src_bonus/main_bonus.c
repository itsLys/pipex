/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:49:09 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 08:41:09 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <unistd.h>

// int	spawn_last_child(t_pipe *data)
// {
// 	pid_t	pid;
// 	int		fd;
//
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		fd = open(data->file[1], O_WRONLY | O_TRUNC | O_CREAT,
// 				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
// 		if (fd == ERROR)
// 			handle_error(FAILIURE, data->file[1], data);
// 		close(data->pipe_fd[PIPE_WR]);
// 		dup2(fd, STDOUT);
// 		close(fd);
// 		dup2(data->pipe_fd[PIPE_RD], STDIN);
// 		close(data->pipe_fd[PIPE_RD]);
// 		ft_execvpe(data->av[1][0], data->av[1], data->envp);
// 		handle_error(CMD_NOT_FOUND, data->av[1][0], data);
// 	}
// 	else if (pid == ERROR)
// 		return (ERROR);
// 	return (pid);
// }

int spawn_middle_children(t_pipe *data)
{
	pid_t	pid;
	int		fd;

	while ()
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	*data;
	int		exit_status;

	if (ac < 5)
		return (FAILIURE);
	data = ft_calloc(1, sizeof(t_pipe));
	if (data == NULL)
		return (FAILIURE);
	parse_args(ac, av, envp, data);
	pipe(data->pipe_fd);
	data->pid[FIRST_CHILD] = spawn_first_child(data);
	spawn_middle_children(data);
	data->pid[LAST_CHILD] = spawn_last_child(data);
	close_pipe(data->pipe_fd);
	waitpid(data->pid[FIRST_CHILD], &data->status[FIRST_CHILD], 0);
	waitpid(data->pid[LAST_CHILD], &data->status[LAST_CHILD], 0);
	exit_status = SUCCESS;
	if (data->pid[0] == ERROR || data->pid[1] == ERROR
		|| WEXITSTATUS(data->status[1]))
		exit_status = FAILIURE;
	if (WEXITSTATUS(data->status[1]) == CMD_NOT_FOUND)
		exit_status = CMD_NOT_FOUND;
	exit_program(data, exit_status);
}

// ac == N + 3
// ./pipex f1 cmd1 cmd2 ... cmdN f2
// first command is av[2] 
// last command is av[ac - 1] 
