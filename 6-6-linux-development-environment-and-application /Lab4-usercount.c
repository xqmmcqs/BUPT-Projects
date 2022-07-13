#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pipe(fd);
    if (!fork())
    {
        close(fd[0]);
        int passwd_fd = open("/etc/passwd", O_RDONLY);
        dup2(passwd_fd, 0);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("grep", "grep", "-v", "usr", NULL);
    }
    if (!fork())
    {
        close(fd[1]);
        int r_fd = open("r.txt", O_WRONLY | O_CREAT, 0777);
        dup2(fd[0], 0);
        close(fd[0]);
        dup2(r_fd, 1);
        execlp("wc", "wc", "-l", NULL);
    }
    wait(NULL);
    close(fd[0]);
    close(fd[1]);
    execlp("cat", "cat", "r.txt", NULL);
}