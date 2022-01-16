#include <bits/stdc++.h>
using namespace std;
int main()
{
    int T = 1000;
    while (T--)
    {
        system("./data >in.in");
        system("./main <in.in >out.out");
        system("./main1 <in.in >out1.out");
        if (system("diff out.out out1.out"))
            break;
        puts("Correct");
    }
    return 0;
}
