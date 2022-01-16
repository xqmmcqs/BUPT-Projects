//
// Created by xqmmcqs on 2020/10/7.
//

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main()
{
    Stack s1, s2;
    initStack(&s1);
    initStack(&s2);
    while (true)
    {
        char t = getchar();
        if (t == '#')break; // 输入结束
        if (t == ' ' || t == '\n' || t == '\t')continue; // 跳过空白字符
        if (t != '0' && t != '1')
        {
            puts("Please check your input.");
            exit(1);
        }
        pushStack(&s1, t - '0');
    }
    while (!isStackEmpty(s1))
    {
        int num = 0;
        for (int i = 0; !isStackEmpty(s1) && i < 3; ++i) // 取出三个二进制数位，计算对应的八进制数位
            num = num + (1 << i) * popStack(&s1);
        pushStack(&s2, num); // 将计算结果入栈
//        printf("%d ", num);
    }
    while (!isStackEmpty(s2) && getStackTop(s2) == 0) // 去除前导0
        popStack(&s2);
    if (isStackEmpty(s2)) // 如果串全为0，则输出一个0
        putchar('0');
    while (!isStackEmpty(s2)) // 逆序输出八进制数字串
        printf("%d", popStack(&s2));
    puts("");
    destroyStack(&s1);
    destroyStack(&s2);
    return 0;
}
