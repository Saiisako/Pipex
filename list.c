/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:22:10 by skock             #+#    #+#             */
/*   Updated: 2025/01/29 12:22:34 by skock            ###   ########.fr       */
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
	new->next = NULL;
	new->index = index++;
	pipex->index++;
	return (new);
}
