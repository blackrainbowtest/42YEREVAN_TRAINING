#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	int		i = 0;
	int		exit_code = 0;
	int		prev_fds = -1;
	int		status;
	int		fds[2];
	pid_t	pid;

	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fds) == -1)
			return (1);
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
			if (prev_fds != -1)
			{
				if (dup2(prev_fds, STDIN_FILENO) == -1)
					exit (1);
				close(prev_fds);
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
		if (prev_fds != -1)
			close(prev_fds);
		if (cmds[i + 1])
		{
			close(fds[1]);
			prev_fds = fds[0];
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
	char	*cmd1[] = {"ls", "-l", NULL};
	char	*cmd2[] = {"grep", "c", NULL};
	char	*cmd3[] = {"wc", "-l", NULL};

	char	**cmds[] = {cmd1, cmd2, cmd3, NULL};
	int		res = picoshell(cmds);
	printf("Exit_code %d\n", res);
	return (0);
}
