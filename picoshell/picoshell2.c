#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
	int		exit_code;
	int		i;
	int		status;
	int		prev_fd;
	int		fds[2];
	pid_t	pid;

	exit_code = 0;
	prev_fd = -1;
	i = 0;
	while(cmds[i])
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
			return (1)
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
	char *cmd1[] = {"ls", "-l", NULL};
	char *cmd2[] = {"grep", "c", NULL};
	char *cmd3[] = {"wc", "-l", NULL};

	char **cmds[] = {cmd1, cmd2, cmd3, NULL};
	int	res = picoshell(cmds);
	printf("exit_code %d\n", res);
	return (0);
}