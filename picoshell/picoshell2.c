#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{

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