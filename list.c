/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:22:10 by skock             #+#    #+#             */
/*   Updated: 2025/02/01 12:51:43 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_lstadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next != NULL)
		{
			last = last->next;
		}
		last->next = new;
	}
}

t_cmd	*ft_lstnew(t_pipex *pipex, char **av)
{
	t_cmd		*new;
	static int	index = 0;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = ft_split(av[pipex->index], ' ');
	new->cmd_path = get_cmd_path(pipex, new->args[0]);
	new->pid = -2;
	new->next = NULL;
	new->index = index++;
	pipex->index++;
	return (new);
}

void free_lst(t_cmd *lst)
{
	t_cmd *temp;

	while (lst)
	{
		temp = lst->next;
		free(lst);
		lst = temp;
	}
}

void	fill_cmd_lst(t_pipex *pipex, char **av, int ac)
{
	t_cmd	*new_cmd;

	while (pipex->index < ac - 1)
	{
		new_cmd = ft_lstnew(pipex, av);
		if (!new_cmd)
			return ;
		ft_lstadd_back(&pipex->cmd_lst, new_cmd);
	}
	pipex->status = 0;
	if (!pipex->cmd_lst)
	{
		ft_putstr_fd("ERROR WHILE TRYING TO FILL LIST", 2);
		free_lst(pipex->cmd_lst);
		exit(1);
	}
}
