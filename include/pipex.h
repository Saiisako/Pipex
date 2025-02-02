/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 13:47:49 by skock             #+#    #+#             */
/*   Updated: 2025/02/02 14:02:49 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "library/libft/libft.h"
# include <sys/wait.h>
# include <stdbool.h>
# include <errno.h>
# define HERE_DOC ".here_doc_temp"

typedef struct s_cmd
{
	char			*cmd_path;
	char			**args;
	pid_t			pid;
	int				index;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_pipex
{
	char	**env;
	int		index;
	int		status;
	int		infile_fd;
	int		outfile_fd;
	bool	is_here_doc;
	t_cmd	*cmd_lst;
}			t_pipex;

// PARSING
void	parsing_path(char **env, t_pipex *pipex);
char	*get_cmd_path(t_pipex *pipex, char *cmd);
// LIST
void	fill_cmd_lst(t_pipex *pipex, char **av, int ac);
void	ft_lstadd_back(t_cmd **lst, t_cmd *new);
t_cmd	*ft_lstnew(t_pipex *pipex, char **av);
// FREE
void	free_pipex_tab(char **args);
void	free_pipex_exit(t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	free_lst(t_cmd *cmd);
void	free_lst_all(t_cmd *lst);
void	exec_error(t_pipex *pipex, t_cmd *current);
// EXEC
void	execute_cmd(t_pipex *pipex, char **env);
void	child_process(t_pipex *pipex, t_cmd *cmd, int *fd, char **env);
void	parent_process(t_pipex *pipex, int *fd);
void	wait_all(t_pipex *pipex);
// HERE_DOC
void	here_doc(t_pipex *pipex, char **av);
void	is_here_doc(t_pipex *pipex, char **av, int ac);
void	update_perror_status(t_pipex *pipex, char **av, int ac);

#endif