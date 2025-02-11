/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:21:23 by skock             #+#    #+#             */
/*   Updated: 2025/02/07 15:20:20 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_pipex_tab(char **arg)
{
	int	i;

	i = 0;
	if (!arg)
		return ;
	while (arg[i])
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}

void	exec_error(t_pipex *pipex, t_cmd *current)
{
	ft_putstr_fd("Error: command not found: ", 2);
	ft_putstr_fd(current->args[0], 2);
	ft_putstr_fd("\n", 2);
	free_lst_all(pipex->cmd_lst);
	free_pipex_tab(pipex->env);
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit(127);
	else
		exit(1);
}

void	free_lst_all(t_cmd *lst)
{
	t_cmd	*tmp;

	while (lst)
	{
		tmp = lst->next;
		if (lst->cmd_path)
			free(lst->cmd_path);
		if (lst->args)
			free_pipex_tab(lst->args);
		free(lst);
		lst = tmp;
	}
}

void	free_pipex(t_pipex *pipex)
{
	free_lst_all(pipex->cmd_lst);
	free_pipex_tab(pipex->env);
}

void	free_pipex_exit(t_pipex *pipex)
{
	free_lst_all(pipex->cmd_lst);
	free_pipex_tab(pipex->env);
	exit(1);
}
