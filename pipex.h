/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:42:03 by skock             #+#    #+#             */
/*   Updated: 2025/01/30 13:13:01 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "library/libft/libft.h"
# include <sys/wait.h>
# include <stdbool.h>
# include <errno.h>
# define HERE_DOC ".here_doc_temp"

typedef	struct s_cmd
{
	char			*cmd_path;
	char			**args;
	pid_t			pid;
	int				index;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_pipex
{
	char	**env;
	int		index;
	int		status;
	int		infile_fd;
	int		outfile_fd;
	bool	is_here_doc;
	t_cmd	*cmd_lst;
}				t_pipex;


// PARSING
void	parsing_path(char **env, t_pipex *pipex);
void	is_here_doc(t_pipex *pipex, char **av, int ac);
char	*get_cmd_path(t_pipex *pipex, char *cmd);
// LIST
void		fill_cmd_lst(t_pipex *pipex, char **av, int ac);
void	ft_lstadd_back(t_cmd **lst, t_cmd *new);
t_cmd	*ft_lstnew(t_pipex *pipex, char **av);
// FREE
void	free_pipex_tab(t_pipex *pipex);
// PRINT

#endif