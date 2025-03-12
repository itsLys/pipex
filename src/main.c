/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:49:09 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/11 00:02:11 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_pipe	*data;

	if (ac != 5)
		return (FAILIURE);
	data = ft_calloc(1, sizeof(t_pipe));
	if (data == NULL)
		return (FAILIURE);
	parse_args(av, envp, data);
	pipe(data->pipe_fd);
	data->pid[FIRST_CHILD] = spawn_first_child(data);
	data->pid[LAST_CHILD] = spawn_last_child(data);
	close_pipe(data->pipe_fd);
	waitpid(data->pid[LAST_CHILD], &data->status, 0);
	wait(NULL);
	if (data->pid[FIRST_CHILD] == ERROR || data->pid[LAST_CHILD] == ERROR)
		handle_error(FAILIURE, "fork", data);
	exit_program(data, WEXITSTATUS(data->status));
}
