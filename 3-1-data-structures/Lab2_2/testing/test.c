#include <stdio.h>
#include <string.h>

const int MAXN = 1e6 + 10;
int n;
char a[MAXN];

int main()
{
    scanf("%s", a + 1);
    n = strlen(a + 1) - 1;
    for (int i = 1; i <= n / 2; ++i)
        if (a[i] != a[n - i + 1])
        {
            puts("NO");
            return 0;
        }
    puts("YES");
    return 0;
}