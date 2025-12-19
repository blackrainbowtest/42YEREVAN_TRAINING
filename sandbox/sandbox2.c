#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

static pid_t	child_pid;

void alarm_handler(int	sig)
{
	(void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	struct sigaction sa;
	int		status;

	sa.sa_handler = alarm_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		f();
		_exit (0);
	}
	child_pid = pid;
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{

	}
	if (WIFEXITED(status))
	{

	}
	if (WIFSIGNALED(status))
	{
		
	}
	return (-1);
}