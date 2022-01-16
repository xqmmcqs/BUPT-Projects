//
// Created by xqmmcqs on 2020/10/7.
//

#ifndef LAB2_1_STACK_H
#define LAB2_1_STACK_H

#include <stdbool.h>

#define STACKINCREASESIZE 100 // 每次分配的栈空间的大小

typedef char ElemType;

typedef struct stack
{
    ElemType * top;
    ElemType * base;
    int stacksize;
} Stack;

/*
 * 操作：初始化栈，分配空间
 * 后件：s指向一个空栈
 */
void initStack(Stack * s);

/*
 * 操作：判断栈是否为空
 * 前件：s是一个栈
 * 后件：如果该栈为空，返回true；否则返回false
 */
bool isStackEmpty(Stack s);

/*
 * 操作：将数据元素入栈
 * 前件：s指向一个栈
 * 后件：如果入栈成功，item成为栈顶元素；如果入栈之前该栈已满，则重新分配空间
 */
void pushStack(Stack * s, ElemType item);

/*
 * 操作：获取栈顶元素
 * 前件：s是一个栈
 * 后件：如果该栈不为空，返回栈顶元素
 */
ElemType getStackTop(Stack s);

/*
 * 操作：栈顶元素出栈
 * 前件：s指向一个栈
 * 后件：如果该栈不为空，栈顶元素出栈，返回这个出栈的元素
 */
ElemType popStack(Stack * s);

/*
 * 操作：释放栈空间
 * 前件：s指向一个栈
 * 后件：释放该栈的空间
 */
void destroyStack(Stack * s);

#endif //LAB2_1_STACK_H