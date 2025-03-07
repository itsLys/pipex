#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#define SUCCESS 0
#define CMD_NOT_FOUND 127
#define ERROR -1
#define FAILIURE 1
#define STDIN 0
#define STDOUT 1
#define PIPE_RD 0
#define PIPE_WR 1

typedef struct s_pipex
{
	char	*file[2];
	int		pipe_fd[2];
	char	**av[2];
	char	**envp;
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;
}			t_pipe;
