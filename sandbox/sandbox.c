#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

static pid_t child_pid;

void alarm_handler(int sig)
{
    (void)sig;  // Suprimir warning de parámetro no usado
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    struct sigaction sa;
    pid_t pid;
    int status;

    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;  // No SA_RESTART: queremos que waitpid sea interrumpido
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    
    pid = fork();
    if (pid == -1)
        return -1;  // Error en fork
    
    if (pid == 0)  // PROCESO HIJO
    {
        f();
        exit(0);  // Función terminó normalmente
    }
    
    child_pid = pid;
    
    alarm(timeout);
    if (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)  // Interrumpido por SIGALRM
        {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);  // Recoger proceso zombie
            
            if (verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return 0;
        }
        return -1;  // Otro tipo de error
    }
    
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return 1;  // Función buena
        }
        else
        {
            if (verbose)
                printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
            return 0;  // Función mala
        }
    }
    
    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (verbose)
            printf("Bad function: %s\n", strsignal(sig));
        return 0;  // Función mala
    }
    
    return -1;  // Estado no reconocido
}
