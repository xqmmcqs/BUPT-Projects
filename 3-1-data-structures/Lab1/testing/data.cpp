#include <bits/stdc++.h>
using namespace std;

const int LIMIT = 98;

int main()
{
    srand(time(0));
    int n, x, y;
    n = rand() % LIMIT + 2;
    x = rand() % n + 1;
    y = rand() % (n * 3) + 2;
    printf("%d %d %d\n", n, x, y);
    return 0;
}
