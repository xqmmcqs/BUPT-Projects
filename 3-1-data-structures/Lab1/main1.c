#include <stdio.h>

int main()
{
    int n, x, y;
    scanf("%d%d%d", &n, &x, &y);
    int ans = 0;
    for (int i = 2; i <= n; ++i)
        ans = (ans + y) % i;
    ans = (ans + x - 1) % n + 1;
    ans == 1 ? puts("Yes") : puts("No");
    printf("%d\n", ans);
    return 0;
}