#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int n;
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("Failed!\n");
        return 0;
    }
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Failed!\n");
        return 1;
    }
    if (pid == 0)
    {
        while (n != 1)
        {
            printf("%d ", n);
            if (n & 1)
                n = n * 3 + 1;
            else
                n /= 2;
        }
        printf("%d\n", n);
    }
    else
    {
        wait(NULL);
        printf("Done.\n");
    }
    return 0;
}
