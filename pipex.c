/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:40:45 by skock             #+#    #+#             */
/*   Updated: 2025/01/27 15:43:33 by skock            ###   ########.fr       */
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
		printf("node %d {%s}\n", temp->index, temp->cmd_path);
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
	new->args = ft_split(av[pipex->index], ' ');
	new->cmd_path = get_cmd_path(pipex, new->args[0]);
	new->next = NULL;
	new->index = index++;
	pipex->index++;
	return (new);
}

void	free_pipex_tab(t_pipex *pipex)
{
	int	i;
	
	i = 0;

	while (pipex->env[i])
	{
		free(pipex->env[i]);
		i++;
	}
	free(pipex->env);
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
			pipex->env = malloc(sizeof(char *) * (count + 1));
			j = 0;
			while (path[j])
			{
				pipex->env[j] = ft_strjoin(path[j], "/");
				free(path[j]);
				j++;
			}
			pipex->env[j] = NULL;
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
	if (!pipex->cmd_lst)
	{
		printf("ERROR WHILE TRYING TO FILL LIST");
		exit(1);
	}
	print_lst(pipex);
}

char	*get_cmd_path(t_pipex *pipex, char *cmd)
{
	int		i;
	char	*res;

	i = 0;
	while (pipex->env[i])
	{
		res = ft_strjoin(pipex->env[i], cmd);
		if (!res)
			return (NULL);
		if (!access(res, X_OK))
			return (res);
		free(res);
		i++;
	}
	return (NULL);
}

void	execute_cmd(t_pipex *pipex)
{
	int		fd[2];
	t_cmd	*temp;

	temp = pipex->cmd_lst->cmd_path;
	while (temp->next)
	{
		if (pipe(fd) == -1)
		{
			perror("Error while creating the pipe.\n");
			exit(1);
		}
		temp->pid = fork();
		if (temp->pid == -1)
			exit(1);
		if (temp->pid == 0)
			parent_process();
		else
			child_process();
		temp = temp->next;
	}
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
		execute_cmd(&pipex);
	}
	free_pipex_tab(&pipex);
}
