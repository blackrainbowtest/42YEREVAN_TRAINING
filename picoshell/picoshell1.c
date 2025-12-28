#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int	picoshell(char **cmds[])
{
	int		i;
	int		status;
	int		prev_fd;
	int		fds[2];
	int		exit_code;
	pid_t	pid;

	i = 0;
	exit_code = 0;
	prev_fd = -1;
	while (cmdss[i])
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
			return (-1);
		}
		if (pid == 0)
		{
			if (prev_fd)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit (1);
				close (prev_fd);
			}
			if (cmds[i + 1])
			{
				close(fds[0]);
				if (dup2(fds[1], STDOUT_FILENO) == -1)
					exit (1);
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
	while (wait(&tatus) != -1)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
	}
	return (exit_code);
}

int	main(void)
{
	char	*cmd1[] = {"ls", "-l", NULL};
	char	*cmd2[] = {"grep", "c", NULL};
	char	*cmd3[] = {"wc", "-l", NULL};

	char	**cmds[] = {cmd1, cmd2, cmd3, NULL};
	int		res = picoshell(cmds);
	printf("Exit_code %d\n", res);
	return (0);

}
