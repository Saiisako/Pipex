/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:55:34 by skock             #+#    #+#             */
/*   Updated: 2025/02/07 14:56:54 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	update_count(char **path, int count)
{
	count = 0;
	while (path[count])
		count++;
	return (count);
}

void	parsing_path(char **env, t_pipex *pipex)
{
	int		i;
	int		j;
	char	**path;
	int		count;

	i = -1;
	path = NULL;
	while (env[++i])
	{
		if (strncmp("PATH=", env[i], 5) == 0)
		{
			path = ft_split(env[i] + 5, ':');
			count = update_count(path, 0);
			pipex->env = malloc(sizeof(char *) * (count + 1));
			j = -1;
			while (path[++j])
			{
				pipex->env[j] = ft_strjoin(path[j], "/");
				free(path[j]);
			}
			pipex->env[j] = NULL;
			break ;
		}
	}
	free(path);
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
		{
			free(res);
			return (NULL);
		}
		if (!access(res, X_OK))
			return (res);
		free(res);
		i++;
	}
	return (NULL);
}
