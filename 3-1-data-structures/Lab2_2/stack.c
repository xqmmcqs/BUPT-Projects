//
// Created by xqmmcqs on 2020/10/7.
//

#include <string.h>
#include <stdlib.h>
#include "stack.h"

// 初始化栈，分配空间
void initStack(Stack * s)
{
    s->base = (ElemType *) malloc(STACKINCREASESIZE * sizeof(ElemType));
    if (!s->base)exit(1); // 空间分配失败
    s->top = s->base;
    s->stacksize = STACKINCREASESIZE;
}

// 判断栈是否为空
bool isStackEmpty(Stack s)
{
    return s.base == s.top;
}

// 将数据元素入栈
void pushStack(Stack * s, ElemType item)
{
    if (s->top - s->base == s->stacksize) // 栈满
    {
        s->base = (ElemType *) realloc(s->base, (s->stacksize + STACKINCREASESIZE) * sizeof(ElemType));
        // 分配更多的空间
        if (!s->base)exit(1); // 空间分配失败
        s->top = s->base + s->stacksize;
        s->stacksize += STACKINCREASESIZE;
    }
    *(s->top++) = item; // 入栈
}

// 栈顶元素出栈
ElemType popStack(Stack * s)
{
    if (isStackEmpty(*s))exit(1); // 如果栈为空，返回异常
    return *(--s->top);
}

// 释放栈空间
void destroyStack(Stack * s)
{
    free(s->base);
    s->base = s->top = NULL;
    s->stacksize = 0;
}
