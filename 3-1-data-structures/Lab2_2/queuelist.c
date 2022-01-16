//
// Created by xqmmcqs on 2020/10/14.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queuelist.h"

// 初始化队列
void initQueue(Queue * q)
{
    Node * newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL)
        exit(1);
    q->front = q->rear = newNode;
    newNode->item = 0; // 创建空的头结点
    newNode->next = NULL;
}

// 判断队列是否为空
bool isQueueEmpty(Queue q)
{
    return q.front == q.rear;
}

// 将数据元素入队
void pushQueue(Queue * q, ElemType item)
{
    Node * newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) // 空间分配失败
        exit(1);
    newNode->item = item;
    newNode->next = NULL; // 将newNode作为链表的尾元素
    q->rear->next = newNode;
    q->rear = newNode; // 将队列尾指向newNode
}

// 队首元素出队
ElemType popQueue(Queue * q)
{
    Node * nextNode = q->front->next;
    ElemType item = nextNode->item; // 出队的是队首指针的下一个元素
    free(q->front); // 释放空间
    q->front = nextNode;
    return item;
}

// 释放队列空间
void destroyQueue(Queue * q)
{
    while (!isQueueEmpty(*q)) // 如果队列不为空，则删除队列中剩余的节点
        popQueue(q);
    free(q->front);
    q->front = q->rear = NULL;
}
