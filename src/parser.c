/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihajji <ihajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:52:08 by ihajji            #+#    #+#             */
/*   Updated: 2025/03/10 23:52:09 by ihajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	parse_args(char **av, char **envp, t_pipe *data)
{
	data->file[0] = av[1];
	data->file[1] = av[4];
	data->av[0] = ft_tokenize(av[2]);
	data->av[1] = ft_tokenize(av[3]);
	data->envp = envp;
	if (data->av[0] == NULL || data->av[1] == NULL)
		handle_error(FAILIURE, "malloc", data);
}
