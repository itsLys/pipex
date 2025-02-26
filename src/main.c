#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define SUCCESS 0
#define ERROR -1

typedef	struct s_pipex
{
	int		fd_in;
	int		fd_out;
	char	cmd_paths[2];

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
char **ft_getpath(char *path)
{
	char **path_list;
	char *tmp;
	int i;

	path_list = ft_split(path, ':');
	i = 0;
	while (path_list[i])
	{
		path_list[i] = ft_strjoin(path_list[i], "/");
		i++;
	}
	return path_list;
}

int main(int ac, char **av, char **ep)
{
	char *path;
	char **path_list;

	if (ac != 5)
		return (ERROR);
	path = ft_getenv(__environ, "PATH=");
	printf("path:	%s\n", path);
	if (path == NULL)
		return (ERROR);
	path_list = ft_getpath(path);
	int i = 0;
	while (path_list[i])
	{
		printf("	%s\n", path_list[i++]);
	}
}
// Hello1
// Hello
// NOTE:	check with access the existence of cmd1, and cmd2
// 			check the existence of the file1 and file2
//
// suppose input is valid
// NOTE: FIX LATER
//
// search for the PATH= variable in environ
// 	search for the command1 in the path environ
// 	search for the command2 in the path environ
