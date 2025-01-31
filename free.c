/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:21:23 by skock             #+#    #+#             */
/*   Updated: 2025/01/31 13:46:08 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_pipex_tab(char **arg)
{
	int	i;

	i = 0;

	while (arg[i])
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}

void	exec_error(t_pipex *pipex, t_cmd *current)
{
	free_pipex_tab(pipex->cmd_lst->args);
	free_pipex_tab(pipex->env);
	free_lst(pipex->cmd_lst);
	ft_putstr_fd("Error: command not found: ", 2);
	ft_putstr_fd(current->args[0], 2);
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit (127);
	else
		exit (1);
}

void	free_lst_all(t_cmd *lst)
{
	t_cmd	*tmp;
	int		i;

	while (lst)
	{
		tmp = lst->next;

		if (lst->cmd_path)
			free(lst->cmd_path);

		if (lst->args)
		{
			i = 0;
			while (lst->args[i])
			{
				free(lst->args[i]);
				i++;
			}
			free(lst->args);
		}

		free(lst);
		lst = tmp;
	}
}

void	free_pipex(t_pipex *pipex)
{
	free_lst_all(pipex->cmd_lst);
	free_pipex_tab(pipex->env);
}
