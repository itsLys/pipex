/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:30:28 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 06:33:18 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_program(t_pipe *data, int status)
{
	close_pipe(data->pipe_fd);
	if (data->av[0])
		ft_free_vector(data->av[0]);
	if (data->av[1])
		ft_free_vector(data->av[1]);
	free(data);
	exit(status);
}

void	handle_error(int status, char *str, t_pipe *data)
{
	perror(str);
	exit_program(data, status);
}

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
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
