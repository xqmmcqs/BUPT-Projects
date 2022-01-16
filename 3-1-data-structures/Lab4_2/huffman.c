//
// Created by xqmmcqs on 2020/11/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huffman.h"

// 排序比较函数
int cmp(int * a, int * b);

// 编码
void encode(FILE * inputFile, FILE * outputFile)
{
    Node huffmanTree[520]; // 哈夫曼树
    char * huffmanCode[260] = {0}; // 哈夫曼编码表
    char inputString[1100]; // 输入字符串
    char tempString[8200];
    char outputString[1100]; // 输出字符串
    int cnt[260][2]; // 各字符权值
    while (true)
    {
        // 输入字符串，统计各字符的权值
        int n = fread(inputString + 1, sizeof(char), 1024, inputFile); // 读取字符串，获取字符串长度
        if (!n)
            break; // 读到文件结束
        for (int i = 0; i <= 255; ++i)
            cnt[i][0] = i, cnt[i][1] = 0;
        for (int i = 1; i <= n; ++i)
            cnt[inputString[i]][1]++;
        qsort(cnt, 256, sizeof(int) * 2, cmp); // 按照权值从大到小排序
        int tot = 0; // 哈夫曼树中叶结点数目
        for (int i = 0; i <= 255; ++i) // 找到第一个权值为0的字符
            if (!cnt[i][1])
            {
                tot = i;
                break;
            }
        if (tot == 1) // 处理只用一种字符的情况
        {
            fwrite(&tot, sizeof(int), 1, outputFile);
            fwrite(inputString + 1, sizeof(char), 1, outputFile);
            fwrite(&n, sizeof(int), 1, outputFile); // 写入原字符串的长度
            continue;
        }
        // 建立哈夫曼树
        for (int i = tot - 1; ~i; --i) // 建立tot个叶结点
        {
            huffmanTree[tot - i].cnt = cnt[i][1];
            huffmanTree[tot - i].data = cnt[i][0];
            huffmanTree[tot - i].lc = huffmanTree[tot - i].rc = huffmanTree[tot - i].parent = 0;
        }
        int t1 = 1, t2 = tot + 1; // 建立剩余的结点，根据单调性，只需要在t1、t1+1、t2、t2+1四个结点中找到权值最小和次小的
        for (int i = tot + 1; i <= 2 * tot - 1; ++i)
        {
            if (t2 == i || t1 + 1 <= tot && huffmanTree[t1].cnt < huffmanTree[t2].cnt &&
                           huffmanTree[t1 + 1].cnt < huffmanTree[t2].cnt) // t1、t1+1是最小和次小的
            {
                huffmanTree[i].cnt = huffmanTree[t1].cnt + huffmanTree[t1 + 1].cnt;
                huffmanTree[i].lc = t1;
                huffmanTree[i].rc = t1 + 1;
                huffmanTree[t1].parent = huffmanTree[t1 + 1].parent = i;
                t1 += 2;
            }
            else if (t1 == tot + 1 || t2 + 1 < i && huffmanTree[t2].cnt < huffmanTree[t1].cnt &&
                                      huffmanTree[t2 + 1].cnt < huffmanTree[t1].cnt) // t2、t2+1是最小和次小的
            {
                huffmanTree[i].cnt = huffmanTree[t2].cnt + huffmanTree[t2 + 1].cnt;
                huffmanTree[i].lc = t2;
                huffmanTree[i].rc = t2 + 1;
                huffmanTree[t2].parent = huffmanTree[t2 + 1].parent = i;
                t2 += 2;
            }
            else // t1、t2是最小和次小的
            {
                huffmanTree[i].cnt = huffmanTree[t1].cnt + huffmanTree[t2].cnt;
                huffmanTree[i].lc = t1;
                huffmanTree[i].rc = t2;
                huffmanTree[t1].parent = huffmanTree[t2].parent = i;
                t1++;
                t2++;
            }
            huffmanTree[i].parent = 0;
        }
        fwrite(&tot, sizeof(int), 1, outputFile); // 写入哈夫曼树中叶结点的数目
        // 将哈夫曼树的一部分信息写进压缩文件中
        for (int i = 1; i <= tot; ++i)
            fwrite(&huffmanTree[i].data, sizeof(char), 1, outputFile);
        for (int i = tot + 1; i <= 2 * tot - 1; ++i)
        {
            fwrite(&huffmanTree[i].lc, sizeof(short), 1, outputFile);
            fwrite(&huffmanTree[i].rc, sizeof(short), 1, outputFile);
        }
        fwrite(&n, sizeof(int), 1, outputFile); // 写入原字符串的长度
        // 从叶结点开始逆向求各字符的哈夫曼编码
        for (int i = 1; i <= tot; ++i)
        {
            int len = tot;
            tempString[len] = 0;
            for (int j = i, k = huffmanTree[j].parent; k; j = k, k = huffmanTree[k].parent)
                tempString[--len] = (huffmanTree[k].rc == j) + '0'; // 为了便于拷贝工作，存储为字符'0'和'1'
            if (!(huffmanCode[huffmanTree[i].data] = (char *) calloc(tot - len + 1, sizeof(char))))
                exit(1);
            strcpy(huffmanCode[huffmanTree[i].data], tempString + len);
        }
        // 将原字符串翻译为哈夫曼编码，每个字节内存储一个'0'/'1'
        int cur = 0;
        for (int i = 1; i <= n; ++i)
        {
            strcpy(tempString + cur, huffmanCode[inputString[i]]);
            cur += strlen(huffmanCode[inputString[i]]);
        }
        // 把不满8位的高位补0
        while (cur % 8)
            tempString[cur++] = '0';
        /*
        for (int i = 0; i < cur; ++i)
            putchar(tempString[i]);
        puts("");
        */
        // 把8个位压缩进一个字节中
        n = 0;
        for (int i = 0; i < cur; i += 8, ++n)
        {
            outputString[n] = 0;
            for (int j = 7; ~j; --j)
                outputString[n] = (outputString[n] << 1) + (tempString[i + j] - '0'); // 后面的位作为高位，便于解码
        }
        fwrite(&n, sizeof(int), 1, outputFile); // 写入编码后的字符串长度
        fwrite(outputString, sizeof(char), n, outputFile); // 写入编码后的字节
        for (int i = 0; i < tot; ++i) // 释放空间
            if (*huffmanCode)
                free(*huffmanCode);
    }
}

// 解码
void decode(FILE * inputFile, FILE * outputFile)
{
    Node huffmanTree[520]; // 哈夫曼树
    char inputString[1100]; // 输入字符串
    while (true)
    {
        int tot; // 哈夫曼树中叶结点数目
        if (!fread(&tot, sizeof(int), 1, inputFile))
            break; // 读到文件结束
        if (tot == 1) // 处理只用一种字符的情况
        {
            char ch;
            int n;
            fread(&ch, sizeof(char), 1, inputFile);
            fread(&n, sizeof(int), 1, inputFile);
            while (n--)fwrite(&ch, sizeof(char), 1, outputFile);
            continue;
        }
        // 读取哈夫曼树的部分信息
        for (int i = 1; i <= tot; ++i)
        {
            fread(&huffmanTree[i].data, sizeof(char), 1, inputFile);
            huffmanTree[i].lc = huffmanTree[i].rc = 0;
        }
        for (int i = tot + 1; i <= 2 * tot - 1; ++i)
        {
            huffmanTree[i].data = -1;
            fread(&huffmanTree[i].lc, sizeof(short), 1, inputFile);
            fread(&huffmanTree[i].rc, sizeof(short), 1, inputFile);
        }
        int len; // 原字符串长度
        int n; // 压缩后字符串长度
        fread(&len, sizeof(int), 1, inputFile);
        fread(&n, sizeof(int), 1, inputFile);
        fread(inputString, sizeof(char), n, inputFile);
        /*
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                char temp = (char)inputString[i] % 2;
                inputString[i] >>= 1;
                putchar(temp + '0');
            }
        }
        puts("");
        */
        int cur = 2 * tot - 1; // 指示当前在哈夫曼树中的结点下标
        int cnt = 0; // 统计已经解码的字符串的长度
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                char temp = (unsigned char) inputString[i] % 2; // 获取最低位
                inputString[i] >>= 1;
                if (temp)
                    cur = huffmanTree[cur].rc;
                else
                    cur = huffmanTree[cur].lc;
                if (!huffmanTree[cur].lc && !huffmanTree[cur].rc) // 找到了叶节点
                {
                    fwrite(&huffmanTree[cur].data, sizeof(char), 1, outputFile);
                    if (++cnt == len) // 解码完了所有字符
                        break;
                    cur = 2 * tot - 1; // 恢复到根结点
                }
            }
        }
    }
}

int cmp(int * a, int * b)
{
    return b[1] - a[1];
}
