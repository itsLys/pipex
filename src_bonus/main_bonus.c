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
	pid_t	last_child;

	if (ac != 6)
	{
		free(data);
		exit(FAILIURE);
	}
	parse_heredoc(av, envp, data);
	spawn_first_heredoc(data);
	last_child = spawn_last_heredoc(data);
	close_pipe(data->pipe_fd);
	waitpid(last_child, &data->status, 0);
	wait(NULL);
	exit_program(data, WEXITSTATUS(data->status));
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	*data;
	pid_t	last_child;

	if (ac < 5)
		return (FAILIURE);
	data = ft_calloc(1, sizeof(t_pipe));
	if (data == NULL)
		return (FAILIURE);
	if (ft_strcmp(av[1], "here_doc") == 0)
		handle_heredoc(ac, av, envp, data);
	parse_args(ac, av, envp, data);
	spawn_first_child(data);
	spawn_middle_children(data);
	last_child = spawn_last_child(data);
	close_pipe(data->pipe_fd);
	waitpid(last_child, &data->status, 0);
	while (wait(NULL) != ERROR)
		;
	exit_program(data, WEXITSTATUS(data->status));
}
