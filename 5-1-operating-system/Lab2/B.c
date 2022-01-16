#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int n;
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("Failed!\n");
        return 0;
    }

    const int SIZE = 4096;
    const char *name = "/buffer";
    int shm_fd;
    void *ptr;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Failed!\n");
        return 1;
    }

    if (pid == 0)
    {
        char *number = (char *)malloc(sizeof(char) * 10);
        while (n != 1)
        {
            sprintf(number, "%d", n);
            sprintf(ptr, "%s ", number);
            ptr += strlen(number) + 1;
            if (n & 1)
                n = n * 3 + 1;
            else
                n /= 2;
        }
        sprintf(number, "%d", n);
        sprintf(ptr, "%s", number);
        ptr += strlen(number);
        free(number);
        munmap(ptr, SIZE);
    }
    else
    {
        wait(NULL);
        printf("%s\n", (char *)ptr);
        printf("Done.\n");
        munmap(ptr, SIZE);
        shm_unlink(name);
    }
    return 0;
}
