//
// Created by xqmmcqs on 2020/11/18.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "huffman.h"

int sizeOfFile(FILE *file);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        char op[5], inputFileName[100], outputFileName[100];
        puts("\n");
        puts("--------------------");
        puts("|** 选择程序功能 **|");
        puts("--------------------");
        puts("(1) 压缩");
        puts("(2) 解压");
        puts("");
        while (true)
        {
            printf("输入你的选择 [1-3]: ");
            scanf("%3s", op);
            if (strlen(op) > 1 || op[0] != '1' && op[0] != '2')
                puts("输入有误，请重新输入");
            else
                break;
        }
        FILE *inputFile = NULL, *outputFile = NULL;
        while (true)
        {
            printf("输入输入文件的路径 （不超过80个字符）: ");
            scanf("%80s", inputFileName);
            if (!(inputFile = fopen(inputFileName, "rb")))
                puts("输入有误，请重新输入");
            else
                break;
        }
        while (true)
        {
            printf("输入输出文件的路径 （不超过80个字符）: ");
            scanf("%80s", outputFileName);
            if (!(outputFile = fopen(outputFileName, "wb")))
                puts("输入有误，请重新输入");
            else
                break;
        }
        if (op[0] == '1')
        {
            puts("开始压缩...");
            encode(inputFile, outputFile);
            int size1 = sizeOfFile(inputFile), size2 = sizeOfFile(outputFile);
            if (size1)
                printf("压缩完成，原文件为 %d 字节，压缩文件为 %d 字节，压缩率 %.6lf%%\n", size1, size2, 100.0 * size2 / (size1));
        }
        else
        {
            puts("开始解压...");
            decode(inputFile, outputFile);
            puts("解压完成");
        }
        fclose(inputFile);
        fclose(outputFile);
    }
    else
    {
        if (argc != 4)
        {
            puts("输入有误，程序即将关闭");
            return 1;
        }
        char *inputFileName = argv[2], *outputFileName = argv[3];
        FILE *inputFile = NULL, *outputFile = NULL;
        if (!(inputFile = fopen(inputFileName, "rb")))
        {
            puts("输入有误，程序即将关闭");
            return 1;
        }
        if (!(outputFile = fopen(outputFileName, "wb")))
        {
            puts("输入有误，程序即将关闭");
            fclose(inputFile);
            return 1;
        }
        if (argv[1][0] == '1')
        {
            puts("开始压缩...");
            encode(inputFile, outputFile);
            int size1 = sizeOfFile(inputFile), size2 = sizeOfFile(outputFile);
            if (size1)
                printf("压缩完成，原文件为 %d 字节，压缩文件为 %d 字节，压缩率 %.6lf%%\n", size1, size2, 100.0 * size2 / (size1));
            fclose(inputFile);
            fclose(outputFile);
        }
        else if (argv[1][0] == '2')
        {
            puts("开始解压...");
            decode(inputFile, outputFile);
            puts("解压完成");
            fclose(inputFile);
            fclose(outputFile);
        }
        else
        {
            puts("输入有误，程序即将关闭");
            fclose(inputFile);
            fclose(outputFile);
            return 1;
        }
    }
    return 0;
}

int sizeOfFile(FILE *file)
{
    fseek(file, 0l, SEEK_END);
    return ftell(file);
}