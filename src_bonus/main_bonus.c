/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:49:09 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/11 00:00:37 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_heredoc(int ac, char **av, char **envp, t_pipe *data)
{
	if (ac != 6)
	{
		free(data);
		exit(FAILIURE);
	}
	parse_heredoc(av, envp, data);
	data->pid[FIRST_CHILD] = spawn_first_heredoc(data);
	data->pid[LAST_CHILD] = spawn_last_heredoc(data);
	exit_program(data, SUCCESS);
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
