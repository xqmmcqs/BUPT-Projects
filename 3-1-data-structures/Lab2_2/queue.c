//
// Created by xqmmcqs on 2020/10/7.
//

#include <string.h>
#include <stdlib.h>
#include "queue.h"

// 初始化队列，分配空间
void initQueue(Queue * q)
{
    q->base = (ElemType *) malloc(QUEUEINCREASESIZE * sizeof(ElemType));
    if (!q->base)exit(1); // 空间分配失败
    q->front = q->rear = q->base;
    q->queuesize = QUEUEINCREASESIZE;
}

// 判断队列是否为空
bool isQueueEmpty(Queue q)
{
    return q.front == q.rear;
}

// 将数据元素入队
void pushQueue(Queue * q, ElemType item)
{
    if (q->rear - q->base == q->queuesize) // 队列满
    {
        int temp1 = q->front - q->base;
        q->base = (ElemType *) realloc(q->base, (q->queuesize + QUEUEINCREASESIZE) * sizeof(ElemType));
        // 分配更多的空间
        if (!q->base)exit(1); // 空间分配失败
        q->front = q->base + temp1;
        q->rear = q->base + q->queuesize;
        q->queuesize += QUEUEINCREASESIZE;
    }
    *(q->rear++) = item; // 入队
}

// 队首元素出队
ElemType popQueue(Queue * q)
{
    if (isQueueEmpty(*q))exit(1); // 如果队列为空，返回异常
    return *(q->front++);
}

// 释放队列空间
void destroyQueue(Queue * q)
{
    free(q->base);
    q->base = q->front = q->rear = NULL;
    q->queuesize = 0;
}
