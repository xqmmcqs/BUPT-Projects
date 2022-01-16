#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int BUFFER_SIZE = 100;
const int UP_DIR = 0;
const int DOWN_DIR = 1;
const int READ_END = 0;
const int WRITE_END = 1;

int main()
{
    int fd[2][2];
    if (pipe(fd[UP_DIR]) == -1 || pipe(fd[DOWN_DIR]) == -1)
    {
        printf("Failed!\n");
        return 1;
    }
    char msg[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Failed!\n");
        return 1;
    }

    if (pid == 0)
    {
        close(fd[UP_DIR][READ_END]);
        close(fd[DOWN_DIR][WRITE_END]);
        read(fd[DOWN_DIR][READ_END], buffer, BUFFER_SIZE);
        for (int i = 0; i < strlen(buffer); ++i)
        {
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
                buffer[i] -= 32;
            else if (buffer[i] >= 'A' && buffer[i] <= 'Z')
                buffer[i] += 32;
        }
        write(fd[UP_DIR][WRITE_END], buffer, strlen(buffer) + 1);
        close(fd[UP_DIR][WRITE_END]);
        close(fd[DOWN_DIR][READ_END]);
    }
    else
    {
        close(fd[UP_DIR][WRITE_END]);
        close(fd[DOWN_DIR][READ_END]);
        fgets(msg, BUFFER_SIZE-1, stdin);
        write(fd[DOWN_DIR][WRITE_END], msg, strlen(msg) + 1);
        read(fd[UP_DIR][READ_END], buffer, BUFFER_SIZE);
        puts(buffer);
        close(fd[UP_DIR][READ_END]);
        close(fd[DOWN_DIR][WRITE_END]);
    }
    return 0;
}
