#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
    pid_t pid;
    int pipefd[2];      // Pipe actual
    int prev_fd = -1;   // Descriptor del pipe anterior
    int status;
    int exit_code = 0;
    int i = 0;
    
    while (cmds[i])
    {
        if (cmds[i + 1] && pipe(pipefd) == -1)
            return 1;
        pid = fork();
        if (pid == -1)
        {
            if (cmds[i + 1])
            {
                close(pipefd[0]);
                close(pipefd[1]);
                return 1;
            }
        }
        if (pid == 0)  // PROCESO HIJO
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            if (cmds[i + 1])
            {
                close(pipefd[0]);  // Cerrar read end (no lo necesitamos)
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(pipefd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);  // Solo se ejecuta si execvp falla
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (cmds[i + 1])
        {
            close(pipefd[1]);     // Cerrar write end
            prev_fd = pipefd[0];  // Guardar read end para siguiente iteración
        }
        i++;
    }
    while (wait(&status) != -1)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            exit_code = 1;
    }
    return exit_code;
}

int main(int argc, char **argv)
{
	printf("pico = %d\n", picoshell(&argv));
}
