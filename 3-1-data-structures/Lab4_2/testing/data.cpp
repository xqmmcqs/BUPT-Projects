#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    srand(time(0));
    FILE *file = fopen("./raw.in", "wb");
    int n = atoi(argv[1]), m = atoi(argv[2]);
    for (int i = 1; i <= n; ++i)
        fprintf(file, "%c", rand() % m + 32);
    fclose(file);
    return 0;
}
