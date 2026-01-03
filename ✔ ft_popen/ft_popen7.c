/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramarak <aramarak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:30:04 by aramarak          #+#    #+#             */
/*   Updated: 2025/12/27 19:30:04 by aramarak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		fds[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fds) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if (dup2(fds[1], STDOUT_FILENO) == -1)
				exit (1);
		}
		else
		{
			if (dup2(fds[0], STDIN_FILENO) == -1)
				exit (1);
		}
		close(fds[0]);
		close(fds[1]);
		execvp(file, argv);
		exit (1);
	}
	if (type == 'r')
	{
		close(fds[1]);
		return (fds[0]);
	}
	else
	{
		close(fds[0]);
		return (fds[1]);
	}
}

