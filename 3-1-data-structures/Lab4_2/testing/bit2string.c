//
// Created by xqmmcqs on 2020/11/18.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    FILE *inputFile = fopen(argv[1], "rb");
    char inputString[1100]; // 输入字符串
    char tempchar;
    while (true)
    {
        int tot; // 哈夫曼树中叶结点数目
        if (!fread(&tot, sizeof(int), 1, inputFile))
            break; // 读到文件结束
        // 读取哈夫曼树的部分信息
        printf("%d\n", tot);
        for (int i = 1; i <= tot; ++i)
        {
            fread(&tempchar, sizeof(char), 1, inputFile);
            printf("%c\n", tempchar);
        }
        for (int i = tot + 1; i <= 2 * tot - 1; ++i)
        {
            fread(&tempchar, sizeof(short), 1, inputFile);
            printf("%d ", tempchar);
            fread(&tempchar, sizeof(short), 1, inputFile);
            printf("%d\n", tempchar);
        }
        int len; // 原字符串长度
        int n;   // 压缩后字符串长度
        fread(&len, sizeof(int), 1, inputFile);
        fread(&n, sizeof(int), 1, inputFile);
        printf("%d %d\n", len, n);
        fread(inputString, sizeof(char), n, inputFile);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                char temp = (unsigned char)inputString[i] % 2;
                inputString[i] >>= 1;
                putchar(temp + '0');
            }
        }
        puts("");
    }
}