#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int LRU(int ins_num, int frame_num, int *address)
{
    int tot_miss = 0;
    int *frame = (int *)malloc(frame_num * sizeof(int));
    int *count = (int *)malloc(frame_num * sizeof(int));
    memset(frame, -1, frame_num * sizeof(int));
    memset(count, 0, frame_num * sizeof(int));
    for (int i = 0; i < ins_num; ++i)
    {
        int max_index = 0, max_count = 0;
        bool hit = false;
        for (int j = 0; j < frame_num; ++j)
        {
            if (frame[j] == address[i])
            {
                hit = true;
                max_index = j;
                break;
            }
            if (frame[j] == -1) // 当前帧为空
            {
                max_index = j;
                break;
            }
            if (count[j] > max_count)
            {
                max_count = count[j];
                max_index = j;
            }
        }
        for (int i = 0; i < frame_num; ++i)
            count[i]++;
        count[max_index] = 0;
        if (!hit)
        {
            tot_miss++;
            frame[max_index] = address[i];
        }
    }
    free(frame);
    free(count);
    return tot_miss;
}

int FIFO(int ins_num, int frame_num, int *address)
{
    int tot_miss = 0;
    int *frame = (int *)malloc(frame_num * sizeof(int));
    int ptr = 0;
    memset(frame, -1, frame_num * sizeof(int));
    for (int i = 0; i < ins_num; ++i)
    {
        bool hit = false;
        for (int j = 0; j < frame_num; ++j)
        {
            if (frame[j] == address[i])
            {
                hit = true;
                break;
            }
        }
        if (!hit)
        {
            tot_miss++;
            frame[ptr] = address[i];
            ptr = (ptr + 1) % frame_num;
        }
    }
    free(frame);
    return tot_miss;
}

int Optimal(int ins_num, int frame_num, int *address)
{
    int tot_miss = 0;
    int *frame = (int *)malloc(frame_num * sizeof(int));
    memset(frame, -1, frame_num * sizeof(int));
    int **next = (int **)malloc(ins_num * sizeof(int *));
    for (int i = 0; i < ins_num; ++i)
        next[i] = (int *)malloc(ins_num * sizeof(int));
    memset(next[ins_num - 1], 0x3f3f3f3f, ins_num * sizeof(int));
    for (int i = ins_num - 2; ~i; --i)
        for (int j = 0; j < ins_num; ++j)
        {
            if (address[i + 1] == j)
                next[i][j] = i + 1;
            else
                next[i][j] = next[i + 1][j];
        }
    for (int i = 0; i < ins_num; ++i)
    {
        int max_index = 0, max_count = 0;
        bool hit = false;
        for (int j = 0; j < frame_num; ++j)
        {
            if (frame[j] == address[i])
            {
                hit = true;
                break;
            }
            if (frame[j] == -1)
            {
                max_index = j;
                break;
            }
            if (next[i][frame[j]] > max_count)
            {
                max_count = next[i][frame[j]];
                max_index = j;
            }
        }
        if (!hit)
        {
            tot_miss++;
            frame[max_index] = address[i];
        }
    }
    free(frame);
    for (int i = 0; i < ins_num; ++i)
        free(next[i]);
    free(next);
    return tot_miss;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    int ins_num = 300;
    int page_size = 15;
    int frame_num = 5;
    if (argc > 1)
    {
        ins_num = atoi(argv[0]);
        page_size = atoi(argv[1]);
        frame_num = atoi(argv[2]);
    }
    int seq = 70, pre = 10, sub = 20;
    int current_address = 0;
    int *address = (int *)malloc(ins_num * sizeof(int));
    for (int i = 0; i < ins_num; ++i)
    {
        address[i] = current_address;
        int x = rand() % (seq + pre + sub);
        if (x < seq)
            current_address++;
        else if (x < seq + pre)
            current_address = rand() % (current_address + 1);
        else
        {
            if (current_address == ins_num - 1)
                current_address++;
            else
                current_address += rand() % (ins_num - 1 - current_address) + 1;
        }
        current_address %= ins_num;
    }
    for (int i = 0; i < ins_num; ++i)
        address[i] /= page_size;
    // for (int i = 0; i < ins_num; ++i)
    //     printf("%d ", address[i]);
    // puts("");
    int ans1 = LRU(ins_num, frame_num, address);
    int ans2 = FIFO(ins_num, frame_num, address);
    int ans3 = Optimal(ins_num, frame_num, address);
    printf("%lf %lf %lf\n", (double)ans1 / ins_num, (double)ans2 / ins_num, (double)ans3 / ins_num);
    free(address);
    return 0;
}