/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:40:45 by skock             #+#    #+#             */
/*   Updated: 2025/02/07 14:48:11 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (pipex->outfile_fd > 2)
		close(pipex->outfile_fd);
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
		if (!temp->args[0])
			free_pipex_exit(pipex);
		if (pipe(fd) < 0)
		{
			perror("Error while creating the pipe.\n");
			free_pipex(pipex);
			exit(1);
		}
		temp->pid = fork();
		if (temp->pid == -1)
			free_pipex_exit(pipex);
		if (temp->pid == 0)
			child_process(pipex, temp, fd, env);
		else
			parent_process(pipex, fd);
		temp = temp->next;
	}
	wait_all(pipex);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	pipex.cmd_lst = NULL;
	if (ac >= 5)
	{
		pipex.status = 0;
		parsing_path(env, &pipex);
		is_here_doc(&pipex, av, ac);
		fill_cmd_lst(&pipex, av, ac);
		execute_cmd(&pipex, env);
		if (pipex.status)
		{
			free_pipex(&pipex);
			exit(pipex.status);
		}
		free_pipex(&pipex);
	}
	return (0);
}
