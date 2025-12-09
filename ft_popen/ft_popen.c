#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    int fds[2];  // Array para los extremos del pipe
    pid_t pid;
    
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    
    if (pipe(fds) == -1)
        return -1;
    
    pid = fork();
    if (pid == -1)
    {
        close(fds[0]);
        close(fds[1]);
        return -1;
    }
    
    if (pid == 0)  // PROCESO HIJO
    {
        if (type == 'r')
        {
            if (dup2(fds[1], STDOUT_FILENO) == -1)
                exit(1);
        }
        else  // type == 'w'
        {
            if (dup2(fds[0], STDIN_FILENO) == -1)
                exit(1);
        }
        
        close(fds[0]);
        close(fds[1]);
        
        execvp(file, argv);
        exit(1);  // Solo se ejecuta si execvp falla
    }
    
    if (type == 'r')
    {
        close(fds[1]);
        return fds[0];
    }
    else  // type == 'w'
    {
        close(fds[0]);
        return fds[1];
    }
}
