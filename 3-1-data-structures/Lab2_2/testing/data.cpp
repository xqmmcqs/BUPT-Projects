#include <bits/stdc++.h>

using namespace std;

const int LEN = 1e6;
char a[LEN + 10];

int main()
{
    srand(time(0));
    int n = rand() % LEN + 1;
    for (int i = 1; i <= n / 2; ++i)
        printf("%c", a[i] = rand() % 26 + 'a');
    if (rand() % 2 == 1)
    {
        for (int i = 1; i <= n / 2; ++i)
            printf("%c", a[i]);
    }
    else
    {
        for (int i = n / 2; i >= 1; --i)
            printf("%c", a[i]);
    }
    puts("#");
    return 0;
}
