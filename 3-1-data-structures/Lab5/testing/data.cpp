#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    srand(time(0));
    int n = atoi(argv[1]), m = atoi(argv[2]);
    printf("%d %d\n", n, m);
    for (int i = 1; i <= m; ++i)
    {
        int x = rand() % (n - 1) + 1;
        printf("%d %d %d\n", x, rand() % (n - x) + x + 1, rand() % 100 + 1);
    }
    return 0;
}
