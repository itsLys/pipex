/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:49:09 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 06:35:50 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	data->pid[0] = spawn_first_child(data);
	data->pid[1] = spawn_second_child(data);
	close_pipe(data->pipe_fd);
	waitpid(data->pid[0], &data->status[0], 0);
	waitpid(data->pid[1], &data->status[1], 0);
	exit_status = SUCCESS;
	if (data->pid[0] == ERROR || data->pid[1] == ERROR
		|| WEXITSTATUS(data->status[1]))
		exit_status = FAILIURE;
	if (WEXITSTATUS(data->status[1]) == CMD_NOT_FOUND)
		exit_status = CMD_NOT_FOUND;
	exit_program(data, exit_status);
}
