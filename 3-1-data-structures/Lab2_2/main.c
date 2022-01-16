//
// Created by xqmmcqs on 2020/10/7.
//

#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "queue.h"

int main()
{
    Stack s;
    Queue q;
    initStack(&s);
    initQueue(&q);
    while (true)
    {
        char t = getchar();
        if (t == '#')break;
        pushStack(&s, t);
        pushQueue(&q, t);
    }
    bool flag = true;
    while (!isStackEmpty(s) && !isQueueEmpty(q))
    {
        if (popQueue(&q) != popStack(&s)) // 比较栈顶元素和队首元素
        {
            flag = false;
            break;
        }
    }
    flag ? puts("YES") : puts("NO");
    destroyStack(&s);
    destroyQueue(&q);
    return 0;
}

