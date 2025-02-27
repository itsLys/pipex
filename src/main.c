#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define SUCCESS 0
#define ERROR -1

typedef	struct s_pipex
{
	int		*fd[2];
	char	*cmd_paths[2];

}		t_pipe ;


char *ft_getenv(char **envp, char *var)
{
	int var_len;

	var_len = ft_strlen(var);
	if (envp)
	{
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
	path = ft_getenv(__environ, "PATH=");
	if (path == NULL)
		return (ERROR);
	path_list = ft_getpath(path);
	if (path_list == NULL)
		return (ERROR);
	// while ()
}

int main(int ac, char **av, char **envp)
{
	t_pipe	pipe;

	if (ac != 5)
		return (ERROR);
}
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
