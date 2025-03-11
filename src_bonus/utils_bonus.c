/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:30:28 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/10 23:55:24 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	exit_program(t_pipe *data, int status)
{
	int	i;

	i = 0;
	close_pipe(data->pipe_fd);
	while (data->av[i])
		ft_free_vector(data->av[i++]);
	free(data->av);
	free(data->delim);
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

int	read_stdin(t_pipe *data)
{
	char	*str;

	ft_printf("> ");
	str = get_next_line(STDIN);
	if (str == NULL)
		return (ERROR);
	while (str && ft_strcmp(str, data->delim))
	{
		write(data->pipe_fd[PIPE_WR], str, ft_strlen(str));
		free(str);
		ft_printf("> ");
		str = get_next_line(STDIN);
		if (str == NULL)
			return (ERROR);
	}
	free(str);
	get_next_line(FREE_BUFFER);
	return (SUCCESS);
}
