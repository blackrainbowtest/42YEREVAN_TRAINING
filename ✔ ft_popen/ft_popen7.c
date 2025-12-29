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
	int		pipefd[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
		}
		else
		{
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		execvp(file, argv);
		exit(EXIT_FAILURE);
	}
	if (type == 'r')
	{
		close(pipefd[1]);
		return (pipefd[0]);
	}
	else
	{
		close(pipefd[0]);
		return (pipefd[1]);
	}
}

int	main(void)
{
	// Example usage of ft_popen
	char *const args[] = { "ls", "-l", NULL };
	int fd = ft_popen("ls", args, 'r');
	if (fd == -1)
	{
		perror("ft_popen failed");
		return (EXIT_FAILURE);
	}
	// Read from fd as needed...
	close(fd);
	return (EXIT_SUCCESS);
}
