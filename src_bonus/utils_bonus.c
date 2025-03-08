/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:30:28 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 08:39:43 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"

void	exit_program(t_pipe *data, int status)
{
	int	i;

	i = 0;
	close_pipe(data->pipe_fd);
	while (data->av[i])
		free(data->av[i++]);
	free(data->av);
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


void	parse_args(int ac, char **av, char **envp, t_pipe *data)
{
	int	i;

	i = 2;
	data->file[0] = av[1];
	data->file[1] = av[ac - 1];
	data->av = ft_calloc(ac - 2, sizeof(char **));
	while (i < ac - 2)
	{
		data->av[i] = ft_tokenize(av[i]);
		if (data->av[i] == NULL)
			handle_error(FAILIURE, "malloc", data);
		i++;
	}
	data->envp = envp;
}

// ac == N + 3
// N == ac - 3
// ./pipex f1 cmd1 cmd2 ... cmdN f2
// first command is av[2] 
// last command is av[ac - 1] 
