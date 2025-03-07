#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SUCCESS 0
#define CMD_NOT_FOUND 127
#define ERROR -1
#define FAILIURE 1
#define STDIN 0
#define STDOUT 1
#define PIPE_RD 0
#define PIPE_WR 1

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

void *free_vector(char **split)
{
	int i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
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
			return (free_vector(path_list));
		free(path_list[i]);
		path_list[i++] = tmp;
	}
	return path_list;
}

void	execute_file(char **path_list, char *file, char **av, char **envp)
{
	char	*file_path;
	int		i;

	i = 0;
	while (path_list[i])
	{
		file_path = ft_strjoin(path_list[i++], file);
		if (file_path == NULL)
			return ;
		execve(file_path, av, envp);
		free(file_path);
	}
	perror(file);
}

int ft_execvpe(char *file, char **av, char **envp)
{
	char	*path;
	char	**path_list;

	if (ft_strchr(file, '/') && execve(file, av, envp) == ERROR)
		return ERROR;
	path = ft_getenv(envp, "PATH=");
	if (path == NULL)
		return (ERROR);
	path_list = ft_getpath(path);
	if (path_list == NULL)
		return (ERROR);
	execute_file(path_list, file, av, envp);
	free_vector(path_list);
	return(ERROR);
}
// TODO: generic functions return status, does not exit


void close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void exit_program(t_pipe *data, int status)
{
	close_pipe(data->pipe_fd);
	if (data->av[0])
		free_vector(data->av[0]);
	if (data->av[1])
		free_vector(data->av[1]);
	free(data);
	exit(status);
}

void handle_error(char *str, t_pipe *data)
{
	perror(str);
	exit_program(data, FAILIURE);
}

int	spawn_first_child(t_pipe *data)
{
	pid_t pid;
	int	  fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[0], O_RDONLY);
		if (fd == ERROR)
			handle_error(data->file[0], data);
		close(data->pipe_fd[PIPE_RD]);
		dup2(fd, STDIN);
		close(fd);
		dup2(data->pipe_fd[PIPE_WR], STDOUT);
		close(data->pipe_fd[PIPE_WR]);
		ft_execvpe(data->av[0][0], data->av[0], data->envp);
		exit_program(data, CMD_NOT_FOUND);
	}
	else if (pid == ERROR)
		return (ERROR);
	return (pid);
}

int spawn_second_child(t_pipe *data)
{
	pid_t pid;
	int fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(data->file[1], O_WRONLY | O_TRUNC | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == ERROR)
			handle_error(data->file[1], data);
		close(data->pipe_fd[PIPE_WR]);
		dup2(fd, STDOUT);
		close(fd);
		dup2(data->pipe_fd[PIPE_RD], STDIN);
		close(data->pipe_fd[PIPE_RD]);
		ft_execvpe(data->av[1][0], data->av[1], data->envp);
		exit_program(data, CMD_NOT_FOUND);
	}
	else if (pid == ERROR)
		return (ERROR);
	return pid;
}

void parse_args(char **av, char **envp, t_pipe *data)
{
	data->file[0] = av[1];
	data->file[1] = av[4];
	data->av[0] = ft_tokenize(av[2]);
	data->av[1] = ft_tokenize(av[3]);
	data->envp = envp;
	if (data->av[0] == NULL || data->av[1] == NULL)
		exit_program(data, FAILIURE);
}

int main(int ac, char **av, char **envp)
{
	t_pipe	*data;
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;
	int		exit_status;

	if (ac != 5)
		return (FAILIURE);
	data = ft_calloc(1, sizeof(t_pipe));
	if (data == NULL)
		return (FAILIURE);
	parse_args(av, envp, data);
	pipe(data->pipe_fd);
	pid1 = spawn_first_child(data);
	pid2 = spawn_second_child(data);
	close_pipe(data->pipe_fd);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	exit_status = SUCCESS;
	if (pid1 == ERROR || pid2 == ERROR /* || WEXITSTATUS(status1)  */|| WEXITSTATUS(status2))
		exit_status = FAILIURE;
	if (WEXITSTATUS(status2) == CMD_NOT_FOUND)
		exit_status = CMD_NOT_FOUND;
	exit_program(data, exit_status);
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
