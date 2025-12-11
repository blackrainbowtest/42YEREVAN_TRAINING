#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	pid_t	pid;
	int		i;
	int		exit_code;
	int		status;
	int		fds[2];
	int		prev_fd;

	exit_code = 0;
	i = 0;
	prev_fd = -1;
	while (cmds[i])
	{

		i++;
	}
	while (wait(&status != -1))
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
	int	res = picoshell(cmds);
	printf("exit_code: %d\n", res);
	return (0);
}