#include <stdio.h>
#include <vector>
#include <fstream>
#include "wf.h"

std::vector<order> orders; //命令集合

int main()
{
    std::ifstream file("data.txt");
    order t;
    while (!file.eof())
    {
        file >> t.rw >> t.spendtime;
        orders.push_back(t);
    }

    FILE *out_file = fopen("output.txt", "w");
    simulate(out_file, orders);
    return 0;
}
