/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:18:38 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/08 08:40:24 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include "get_next_line.h"
# define SUCCESS 0
# define CMD_NOT_FOUND 127
# define ERROR -1
# define FAILIURE 1
# define STDIN 0
# define STDOUT 1
# define PIPE_RD 0
# define PIPE_WR 1
# define FIRST_CHILD 0
# define LAST_CHILD 1

// typedef struct s_heredoc
// {
// 	char	*file;
// 	int		pipe_fd[2];
// 	char	*delim;
// 	char	**av[2];
// 	char	**envp;
// 	pid_t	pid[2];
// 	int		status[2];
// }			t_heredoc;

typedef struct s_pipex
{
	char	*file[2];
	int		pipe_fd[2];
	int		cmd_count;
	char	***av;
	char	**envp;
	pid_t	pid[2];
	int		status[2];
	char	*delim;
}			t_pipe;


void	close_pipe(int fd[2]);
void	exit_program(t_pipe *data, int status);
void	handle_error(int status, char *str, t_pipe *data);
void	parse_args(int ac, char **av, char **envp, t_pipe *data);
pid_t	spawn_first_child(t_pipe *data);
void	spawn_middle_children(t_pipe *data);
pid_t	spawn_last_child(t_pipe *data);

#endif // !PIPEX_BONUS_H
