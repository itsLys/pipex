#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#define SUCCESS 0
#define ERROR -1

typedef	struct s_pipex
{
	int		fd[2];
	int		pipe_fd[2];
	char	**av[2];
	int		end1;
	int		end2;
}		t_pipe ;

char *ft_getenv(char **envp, char *var)
{
	int var_len;

	if (envp)
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
	exit(ERROR);
}


int spawn_child(char **av, char **envp, t_pipe data)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == 0)
	{
		close(data.end2);
		close(data.pipe_fd[data.end1]);
		close(data.fd[data.end2]);
		dup2(data.fd[data.end1], data.end1);
		dup2(data.pipe_fd[data.end2], data.end2);
		close(data.pipe_fd[data.end2]);
		ft_execvpe(data.av[data.end1][0], data.av[data.end1], envp);
		// hendle errors
	}
	else if (pid == -1)
		return (ERROR);
	return (SUCCESS);
}
char **parse_av(char *str)
{
	return NULL;
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
	data.fd[0] = open(av[1], O_RDONLY);
	data.fd[1] = open(av[4], O_WRONLY | O_TRUNC);
	data.av[0] = parse_av(av[2]);
	data.av[1] = parse_av(av[3]);
	pipe(data.pipe_fd);
	spawn_child(data.av[0], envp, data);
	spawn_child(data.av[1], envp, data);
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
