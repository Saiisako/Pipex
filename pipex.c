/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:40:45 by skock             #+#    #+#             */
/*   Updated: 2025/01/27 11:46:31 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_lst(t_pipex *pipex)
{
	int		i;
	t_cmd	*temp;

	temp = pipex->cmd_lst;
	i = 0;
	while (temp)
	{
		printf("node %d {%s}\n", temp->index, temp->cmd);
		temp = temp->next;
	}
}

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
	new->cmd = av[pipex->index];
	new->next = NULL;
	new->index = index++;
	pipex->index++;
	return (new);
}

void	free_pipex_tab(t_pipex *pipex)
{
	int	i;
	
	i = 0;

	while (pipex->path[i])
	{
		free(pipex->path[i]);
		i++;
	}
	free(pipex->path);
}

void	parsing_path(char **env, t_pipex *pipex)
{
	int		i;
	int		j;
	char	**path;
	int		count;

	i = 0;
	path = NULL;
	while (env[i])
	{
		if (strncmp("PATH=", env[i], 5) == 0)
		{
			path = ft_split(env[i] + 5, ':');
			count = 0;
			while (path[count])
				count++;
			pipex->path = malloc(sizeof(char *) * (count + 1));
			j = 0;
			while (path[j])
			{
				pipex->path[j] = ft_strjoin(path[j], "/");
				free(path[j]);
				j++;
			}
			pipex->path[j] = NULL;
			break ;
		}
		i++;
	}
	free(path);
}

void	is_here_doc(t_pipex *pipex, char **av)
{
	pipex->index = 0;
	if (!ft_strcmp(av[1], "here_doc"))
	{
		pipex->index = 3;
		printf("here_doc_entering\n");
	}
	else
	{
		pipex->index = 2;
		printf("pipex_entering\n");
	}
}

int	fill_cmd_lst(t_pipex *pipex, char **av, int ac)
{
	t_cmd	*new_cmd;

	while (pipex->index < ac - 1)
	{
		new_cmd = ft_lstnew(pipex, av);
		if (!new_cmd)
		{
			printf("Erreur : allocation mémoire pour un nœud a échoué.\n");
			return (0);
		}
		ft_lstadd_back(&pipex->cmd_lst, new_cmd);
	}
	if (!pipex->cmd_lst)
	{
		printf("ERROR WHILE TRYING TO FILL LIST");
		exit(1);
		return (0);
	}
	print_lst(pipex);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;
	
	pipex.cmd_lst = NULL;
	if (ac == 5)
	{
		parsing_path(env, &pipex);
		is_here_doc(&pipex, av);
		fill_cmd_lst(&pipex, av, ac);
	}
	free_pipex_tab(&pipex);
}
