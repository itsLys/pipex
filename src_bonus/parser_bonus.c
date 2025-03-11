/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:48:40 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/10 23:48:59 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	parse_heredoc(char **av, char **envp, t_pipe *data)
{
	data->delim = ft_strjoin(av[2], "\n");
	if (data->delim == NULL)
		handle_error(FAILIURE, "malloc", data);
	data->av = ft_calloc(3, sizeof(char **));
	if (data->av == NULL)
		handle_error(FAILIURE, "malloc", data);
	data->av[0] = ft_tokenize(av[3]);
	data->av[1] = ft_tokenize(av[4]);
	if (data->av[0] == NULL || data->av[1] == NULL)
		handle_error(FAILIURE, "malloc", data);
	data->file[FILE_OUT] = av[5];
	data->envp = envp;
}

void	parse_args(int ac, char **av, char **envp, t_pipe *data)
{
	int	i;

	i = 0;
	data->file[FILE_IN] = av[1];
	data->file[FILE_OUT] = av[ac - 1];
	data->av = ft_calloc(ac - 2, sizeof(char **));
	while (i < ac - 3)
	{
		data->av[i] = ft_tokenize(av[i + 2]);
		if (data->av[i] == NULL)
			handle_error(FAILIURE, "malloc", data);
		i++;
	}
	data->cmd_count = i;
	data->envp = envp;
}
