/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:47:03 by skock             #+#    #+#             */
/*   Updated: 2025/02/07 15:20:20 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	here_doc(t_pipex *pipex, char **av)
{
	char	*line;
	char	*tmp_lim;
	char	*limiter;

	pipex->infile_fd = open(HERE_DOC, O_CREAT | O_RDWR | O_TRUNC, 0644);
	tmp_lim = ft_strdup(av[2]);
	limiter = ft_strjoin(tmp_lim, "\n");
	free(tmp_lim);
	while (1)
	{
		ft_printf("here_doc> ");
		line = get_next_line(0);
		if (!ft_strcmp(line, limiter))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipex->infile_fd);
		free(line);
	}
	close(pipex->infile_fd);
	pipex->infile_fd = open(HERE_DOC, O_RDONLY);
	free(limiter);
}

void	update_error(t_pipex *pipex, int ac, char **av)
{
	perror(av[ac - 1]);
	pipex->status = 1;
}

void	update_perror_status(t_pipex *pipex, char **av, int ac)
{
	perror(av[ac - 1]);
	pipex->status = 1;
}

void	is_here_doc(t_pipex *pipex, char **av, int ac)
{
	pipex->index = 0;
	if (!ft_strcmp(av[1], "here_doc"))
	{
		pipex->outfile_fd = open(av[ac - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (pipex->outfile_fd < 0)
			update_perror_status(pipex, av, ac);
		pipex->index = 3;
		pipex->is_here_doc = true;
		here_doc(pipex, av);
	}
	else
	{
		pipex->infile_fd = open(av[1], O_RDONLY, 0644);
		if (pipex->infile_fd < 0)
			pipex->infile_fd = open("/dev/null", O_RDONLY);
		pipex->outfile_fd = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (pipex->outfile_fd < 0)
			update_perror_status(pipex, av, ac);
		pipex->is_here_doc = false;
		pipex->index = 2;
	}
}
