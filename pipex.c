/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:40:45 by skock             #+#    #+#             */
/*   Updated: 2025/01/31 11:30:07 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	is_here_doc(t_pipex *pipex, char **av, int ac)
{
	pipex->index = 0;
	if (!ft_strcmp(av[1], "here_doc"))
	{
		pipex->outfile_fd = open(av[ac - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (pipex->outfile_fd < 0)
		{
			perror(av[ac - 1]);
			pipex->status = 1;
		}
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
		{
			perror(av[ac - 1]);
			pipex->status = 1;
		}
		pipex->is_here_doc = false;
		pipex->index = 2;
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
	if (pipex->status != 1)
		pipex->status = 0;
	if (!pipex->cmd_lst)
	{
		ft_putstr_fd("ERROR WHILE TRYING TO FILL LIST", 2);
		free_lst(pipex->cmd_lst);
		exit(1);
	}
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
		{
			return (res);
		}
		free(res);
		i++;
	}
	return (NULL);
}
void	free_lst(t_cmd *lst)
{
	t_cmd	*temp;

	temp = NULL;
	while(temp)
	{
		temp = lst->next;
		free(lst);
		lst = temp;
	}
	return ;
}

void	exec_error(t_pipex *pipex, t_cmd *current)
{
	free_pipex_tab(pipex->cmd_lst->args);
	free_pipex_tab(pipex->env);
	free_lst(pipex->cmd_lst);
	// free(pipex->cmd_lst->cmd_path);
	// free(current);
	// free(pipex);
	ft_putstr_fd("Error: command not found: ", 2);
	ft_putstr_fd(current->args[0], 2);
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit (127);
	else
		exit (1);
}

void	child_process(t_pipex *pipex, t_cmd *cmd, int *fd, char **env)
{
	close(fd[0]);
	dup2(pipex->infile_fd, STDIN_FILENO);
	if (!cmd->next)
		dup2(pipex->outfile_fd, STDOUT_FILENO);
	else
		dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(pipex->infile_fd);
	execve(cmd->cmd_path, cmd->args, env);
	exec_error(pipex, cmd);
}

void	parent_process(t_pipex *pipex, int *fd)
{
	close(fd[1]);
	close(pipex->infile_fd);
	pipex->infile_fd = dup(fd[0]);
	close(fd[0]);
	if (pipex->is_here_doc == true)
	{
		unlink(HERE_DOC);
		pipex->is_here_doc = false;
	}
}

void	wait_all(t_pipex *pipex)
{
	t_cmd	*temp;
	int		tmp;

	temp = pipex->cmd_lst;
	while (temp)
	{
		waitpid(temp->pid, &tmp, 0);
		temp = temp->next;
	}
	if (WIFEXITED(tmp))
		if (WEXITSTATUS(tmp) > pipex->status)
			pipex->status = WEXITSTATUS(tmp);
}

void	execute_cmd(t_pipex *pipex, char **env)
{
	int		fd[2];
	t_cmd	*temp;

	temp = pipex->cmd_lst;
	while (temp)
	{
		if (pipe(fd) < 0)
		{
			perror("Error while creating the pipe.\n");
			exit(1);
		}
		temp->pid = fork();
		if (temp->pid == -1)
			exit(1);
		if (temp->pid == 0)
			child_process(pipex, temp, fd, env);
		else
			parent_process(pipex, fd);
		temp = temp->next;
	}
	wait_all(pipex);
}

void	free_lst_all(t_cmd *lst)
{
	t_cmd	*tmp;
	int		i;

	while (lst)
	{
		tmp = lst->next;

		if (lst->cmd_path)
			free(lst->cmd_path);

		if (lst->args)
		{
			i = 0;
			while (lst->args[i])
			{
				free(lst->args[i]);
				i++;
			}
			free(lst->args);
		}

		free(lst);
		lst = tmp;
	}
}

void	free_pipex(t_pipex *pipex)
{
	free_lst_all(pipex->cmd_lst);
	free_pipex_tab(pipex->env);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;
	
	pipex.cmd_lst = NULL;
	if (ac >= 5)
	{
		parsing_path(env, &pipex);
		is_here_doc(&pipex, av, ac);
		fill_cmd_lst(&pipex, av, ac);
		execute_cmd(&pipex, env);
		if (pipex.status)
		{
			free_pipex(&pipex);
			exit(pipex.status);
		}
	}
	free_pipex(&pipex);
	return (0);
}
