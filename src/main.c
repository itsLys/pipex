#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SUCCESS 0
#define ERROR -1

typedef	struct s_pipex
{
	char	*file[2];
	int		pipe_fd[2];
	char	**av[2];
	char	**envp;
}		t_pipe ;

char *ft_getenv(char **envp, char *var)
{
	int var_len;

	if (envp && var)
	{
		var_len = ft_strlen(var);
		while (*envp)
		{
			if (!ft_strncmp(*envp, var, var_len))
				return (*envp + var_len);
			envp++;
		}
	}
	return (NULL);
}
// test edge cases

void *free_split(char **split)
{
	while (*split)
		free(split++);
	return (NULL);
}

char **ft_getpath(char *path)
{
	char **path_list;
	char *tmp;
	int i;

	path_list = ft_split(path, ':');
	if (path_list == NULL)
		return (NULL);
	i = 0;
	while (path_list[i])
	{
		tmp = ft_strjoin(path_list[i], "/");
		if (tmp == NULL)
			return (free_split(path_list));
		free(path_list[i]);
		path_list[i++] = tmp;
	}
	return path_list;
}

int ft_execvpe(char *file, char **av, char **envp)
{
	char	**path_list;
	char	*path;
	int		i;
	if (ft_strchr(file, '/') && execve(file, av, envp) == ERROR)
	{
		perror("execve");
		exit(ERROR);
	}
	path = ft_getenv(envp, "PATH=");
	if (path == NULL)
		return (ERROR);
	path_list = ft_getpath(path);
	if (path_list == NULL)
		return (ERROR);
	i = 0;
	while (execve(ft_strjoin(path_list[i], file), av, envp) == -1)
		i++;
	perror("execve");
	return(ERROR);
}
// TODO: generic functions return status, does not exit

#define STDIN 0
#define STDOUT 1
#define PIPE_RD 0
#define PIPE_WR 1

// O_WRONLY | O_TRUNC | O_CREAT,
// 				   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
int spawn_first_child(t_pipe data)
{
	pid_t pid;
	int	  fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data.file[0], O_RDONLY);
		if (fd == -1)
			exit(-1);
		close(data.pipe_fd[PIPE_RD]);
		if (dup2(fd, STDIN) == ERROR)
			return ERROR;
		dup2(data.pipe_fd[PIPE_WR], STDOUT);
		close(data.pipe_fd[PIPE_WR]);
		ft_execvpe(data.av[0][0], data.av[0], data.envp);
		exit(ERROR);
	}
	else if (pid == ERROR)
		return ERROR;
	return 0;
}

int spawn_second_child(t_pipe data)
{
	pid_t pid;
	int fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data.file[1], O_WRONLY | O_TRUNC | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == -1)
			exit(-1);
		close(data.pipe_fd[PIPE_WR]);
		dup2(fd, STDOUT);
		dup2(data.pipe_fd[PIPE_RD], STDIN);
		close(data.pipe_fd[PIPE_RD]);
		ft_execvpe(data.av[1][0], data.av[1], data.envp);
		exit(ERROR);
	}
	else if (pid == ERROR)
		return ERROR;
	return 0;
}

char **parse_av(char *str)
{
	char **av;

	av = ft_tokenize(str);
	if (av == NULL)
		return (NULL);
	return av;
}

void close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int main(int ac, char **av, char **envp)
{
	t_pipe	data;

	if (ac != 5)
		return (ERROR);
	data.file[0] = av[1];
	data.file[1] = av[4];
	data.av[0] = parse_av(av[2]);
	data.av[1] = parse_av(av[3]);
	data.envp = envp;
	pipe(data.pipe_fd);
	spawn_first_child(data);
	spawn_second_child(data);
	close_pipe(data.pipe_fd);
	while (wait(NULL) != -1)
		;
}

// Now I have the file1
// now that I waited I have the output, now I need to read it somehow
// I have written in  the pipe, now I need to read from it
// fork another time,
// dup the 0 into the read end of the pipe
// and execute the second command
// and in that procces, dup the 1 into the file2 to save the output
//
//
//
// allocate memory and push to the pointer to a list,
// and ft_free
//
//
//
//
// now that i have path list, I need to figure out where is the command located?
// 	maybe I need to execve it until the return value is not -1;
// NOTE:	check with access the existence of cmd1, and cmd2
// 			check the existence of the file1 and file2
//
// suppose input is valid
// NOTE: FIX LATER
//
// search for the PATH= variable in environ
// 	search for the command1 in the path environ
// 	search for the command2 in the path environ
