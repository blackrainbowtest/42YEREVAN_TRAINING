#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
	pid_t	pid;
	int		fds[2];
	int		i;
	int		exit_code;
	int		prev_fd;
	int		status;

	exit_code = 0;
	prev_fd = -1;
	i = 0;
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fds) == -1)
			return (-1);
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
			{
				close(fds[0]);
				close(fds[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit (1);
				close (prev_fd);
			}
			if (cmds[i + 1])
			{
				close(fds[0]);
				if (dup2(fds[1], STDOUT_FILENO) == -1)
					exit(1);
				close(fds[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit (1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(fds[1]);
			prev_fd = fds[0];
		}
		i++;
	}
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
	}
	return (exit_code);
}

int	main(void)
{
	char	*cmd1[] = {};
	char	*cmd2[] = {};
	char	*cmd3[] = {};

	char	**cmds[] = {cmd1, cmd2, cmd3, NULL};
	int	res = picoshell(cmds);
	printf("exit_code %d\n", res);
	return (1);
}