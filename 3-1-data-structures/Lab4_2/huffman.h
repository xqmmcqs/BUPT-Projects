//
// Created by xqmmcqs on 2020/11/18.
//

#ifndef LAB4_2_HUFFMAN_H
#define LAB4_2_HUFFMAN_H

typedef struct node
{
    char data;
    int cnt;
    short parent, lc, rc;
} Node;

/*
 * 操作：编码
 * 前件：inputFile和outputFile分别指向输入、输出文件
 * 后件：将编码结果输出到outputFile指向的文件中
 */
void encode(FILE * inputFile, FILE * outputFile);

/*
 * 操作：解码
 * 前件：inputFile和outputFile分别指向输入、输出文件
 * 后件：将解码结果输出到outputFile指向的文件中
 */
void decode(FILE * inputFile, FILE * outputFile);

#endif //LAB4_2_HUFFMAN_H