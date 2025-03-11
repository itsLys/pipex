/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:30:28 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/10 23:52:15 by ihajji           ###   ########.fr       */
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
	close(fd[PIPE_RD]);
	close(fd[PIPE_WR]);
}
