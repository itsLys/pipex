/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:18:38 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/10 23:57:37 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/stat.h>
# include <sys/wait.h>
# define PIPE_RD 0
# define PIPE_WR 1
# define FIRST_CHILD 0
# define LAST_CHILD 1
# define FILE_IN 0
# define FILE_OUT 1

typedef struct s_pipex
{
	char	*file[2];
	int		pipe_fd[2];
	char	**av[2];
	char	**envp;
	pid_t	pid[2];
	int		status;
}			t_pipe;

void	close_pipe(int fd[2]);
void	exit_program(t_pipe *data, int status);
void	handle_error(int status, char *str, t_pipe *data);
void	parse_args(char **av, char **envp, t_pipe *data);
pid_t	spawn_first_child(t_pipe *data);
pid_t	spawn_last_child(t_pipe *data);

#endif // !PIPEX_H
