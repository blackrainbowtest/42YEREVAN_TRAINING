#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
    int   i;
    int   prev_fd;
    int   fds[2];
    pid_t pid;

    i = 0;
    prev_fd = -1;
    while (cmds[i])
    {
        if (cmds[i + 1])
        {
            if (pipe(fds) == -1)
                return (1);
        }

        pid = fork();
        if (pid == -1)
        {
            // [ADDED] корректно закрываем только что созданный пайп при ошибке fork
            if (cmds[i + 1]) {
                close(fds[0]);
                close(fds[1]);
            }
            // [UNCHANGED] можно было бы ещё закрыть prev_fd, но его закроет ОС на выходе
            return (1);
        }

        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    _exit(1); // [CHANGED] exit -> _exit
                close(prev_fd);
            }
            if (cmds[i + 1])
            {
                if (dup2(fds[1], STDOUT_FILENO) == -1)
                    _exit(1); // [CHANGED] exit -> _exit
                close(fds[0]);
                close(fds[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            _exit(1); // [CHANGED] exit -> _exit
        }

        if (prev_fd != -1)
            close(prev_fd);

        if (cmds[i + 1])
        {
            close(fds[1]);
            prev_fd = fds[0];
        }
        else
            prev_fd = -1;

        i++;
    }

    while (wait(NULL) > 0)
        ;
    return (0);
}

// int main(void)
// {
//     // ТВОЙ исходный пример:
//     char *cmd1[] = {"ls", "-l", NULL};
//     char *cmd2[] = {"grep", "c", NULL};
//     char *cmd3[] = {"wc", "-l", NULL};
//     char **cmds[] = {cmd1, cmd2, cmd3, NULL};

//     // Если вывод «пропадает», попробуй один из тестов выше
//     // чтобы убедиться, что конвейер работает.

//     return picoshell(cmds);
// }


int	main(void)
{
	// char *cmd1[] = {"ls", "-l", NULL};
	// char *cmd2[] = {"grep", "c", NULL};
	// char *cmd3[] = {"wc", "-l", NULL};

	// char **cmds[] = {cmd1, cmd2, cmd3, NULL};

	сhar *cmd1[] = {"echo", "HELLO", NULL};
	char *cmd2[] = {"wc", "-c", NULL};
	char **cmds[] = {cmd1, cmd2, NULL};
	picoshell(cmds);
	return (0);
}
