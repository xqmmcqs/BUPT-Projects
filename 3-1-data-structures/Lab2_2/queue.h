//
// Created by xqmmcqs on 2020/10/7.
//

#ifndef LAB2_2_QUEUE_H
#define LAB2_2_QUEUE_H

#include <stdbool.h>

#define QUEUEINCREASESIZE 100

typedef char ElemType;

typedef struct queue
{
    ElemType * front;
    ElemType * rear;
    ElemType * base;
    int queuesize;
} Queue;

/*
 * 操作：初始化队列，分配空间
 * 后件：q指向一个空队列
 */
void initQueue(Queue * q);

/*
 * 操作：判断队列是否为空
 * 前件：q是一个队列
 * 后件：如果该队列为空，返回true；否则返回false
 */
bool isQueueEmpty(Queue q);

/*
 * 操作：将数据元素入队
 * 前件：q指向一个队列
 * 后件：如果入队成功，item成为队尾元素；如果入队之前该队列已满，则重新分配空间
 */
void pushQueue(Queue * q, ElemType item);

/*
 * 操作：队首元素出队
 * 前件：q指向一个队列
 * 后件：如果该队列不为空，队首元素出栈，返回这个出队的元素
 */
ElemType popQueue(Queue * q);

/*
 * 操作：释放队列空间
 * 前件：q指向一个队列
 * 后件：释放该队列的空间
 */
void destroyQueue(Queue * q);

#endif //LAB2_2_QUEUE_H