#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

static pid_t child_pid;

void alarm_handler(int sig)
{
	(void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;
	struct sigaction sa;

	sa.sa_handler = alarm_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	pid = fork();
	if (pid == -1)
		return (-1)
	if (pid == 0)
	{
		f();
		exit (0);
	}
	child_pid = pid;
	alarm(timeout);
	if (waitpid(pid, &status, NULL) == -1)
	{

	}
	if (WIFEXITED(status))
	{

	}
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %d\n", strsignal(sig));
		return (0);
	}
	return (-1);
}