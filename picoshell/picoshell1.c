#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
	int		exit_code;
	int		fds[2];
	pid_t	pid;
	int		status;
	int		prev_fd;
	int		i;

	exit_code = 0;
	prev_fd = -1;
	i = 0;
	while (cmds[i])
	{
		i++;
	}
	while (wait(&status) != 1)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
	}
	return (exit_code);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (1);
	printf("pico = %d\n", picoshell(&argv));
}