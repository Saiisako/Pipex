/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:42:03 by skock             #+#    #+#             */
/*   Updated: 2025/01/27 14:40:07 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

typedef	struct s_cmd
{
	char			*cmd_path;
	char			**args;
	int				pid;
	int				index;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_pipex
{
	char	**env;
	int		index;
	int		infile_fd;
	int		outfile_fd;
	t_cmd	*cmd_lst;
}				t_pipex;

# include "library/libft/libft.h"
# include <sys/wait.h>

// PARSING
void	parsing_path(char **env, t_pipex *pipex);
void	is_here_doc(t_pipex *pipex, char **av);
char	*get_cmd_path(t_pipex *pipex, char *cmd);
// LIST
void		fill_cmd_lst(t_pipex *pipex, char **av, int ac);
void	ft_lstadd_back(t_cmd **lst, t_cmd *new);
t_cmd	*ft_lstnew(t_pipex *pipex, char **av);
// FREE
void	free_pipex_tab(t_pipex *pipex);
// PRINT
void	print_lst(t_pipex *pipex);

#endif