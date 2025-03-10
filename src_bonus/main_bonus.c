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

#include "get_next_line.h"
#include "libft.h"
#include "pipex_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	parse_heredoc(char **av, char **envp, t_pipe *data)
{
	data->delim = ft_strjoin(av[2], "\n");
	if (data->delim == NULL)
		handle_error(FAILIURE, "malloc", data);
	data->av[0] = ft_tokenize(av[3]);
	data->av[1] = ft_tokenize(av[4]);
	data->file[1] = av[5];
	data->envp = envp;
}

int	read_stdin(t_pipe *data)
{
	char	*str;

	ft_printf("> ");
	str = get_next_line(STDIN);
	if (str == NULL)
		return (ERROR);
	while (str && ft_strcmp(str, data->delim))
	{
		write (data->pipe_fd[PIPE_WR], str, ft_strlen(str));
		free(str);
		ft_printf("> ");
		str = get_next_line(STDIN);
		if (str == NULL)
			return (ERROR);
	}
	free(str);
	return (SUCCESS);
}

pid_t	spawn_first_heredoc(t_heredoc *data)
{
	pid_t pid;
	char *str;

	pipe(data->pipe_fd);
	if (read_stdin(data) == ERROR)
		handle_error(FAILIURE, "malloc", data)
	close(data->pipe_fd[PIPE_WR]);
	dup2(data->pipe_fd[PIPE_RD], STDIN);
	close(data->pipe_fd[PIPE_RD]);
	pipe(data->pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		close(data->pipe_fd[PIPE_RD]);
		dup2(data->pipe_fd[PIPE_WR], STDOUT);
		close(data->pipe_fd[PIPE_WR]);
		ft_execvpe(data->av[0][0], data->av[0], data->envp);
	}
	return pid;
}

pid_t	spawn_last_heredoc(t_heredoc *data)
{
	pid_t pid;
	int fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file, O_WRONLY | O_APPEND | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		printf("%d\n", fd);
		close(data->pipe_fd[PIPE_WR]);
		dup2(fd, STDOUT);
		close(fd);
		dup2(data->pipe_fd[PIPE_RD], STDIN);
		close(data->pipe_fd[PIPE_RD]);
		ft_execvpe(data->av[1][0], data->av[1], data->envp);
	}
	return pid;
}

void	handle_heredoc(int ac, char **av, char **envp, t_pipe *data)
{
	int			exit_status;

	if (ac != 6)
		exit_program(data, FAILIURE);
	parse_heredoc(av, envp, data);
	data->pid[FIRST_CHILD] = spawn_first_heredoc(data);
	data->pid[LAST_CHILD] = spawn_last_heredoc(data);
	exit_status = 321321;
	exit(exit_status);
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
	if (ft_strcmp(av[1], "here_doc") == 0)
		handle_heredoc(ac, av, envp, data);
	parse_args(ac, av, envp, data);
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
