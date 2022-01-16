//
// Created by xqmmcqs on 2020/9/30.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

// 初始化链表，创建头结点
void initList(List * plist)
{
    *plist = (List) malloc(sizeof(Node));
    if (!*plist)
        exit(1);
    (*plist)->item = 0; // 创建空的头结点
    (*plist)->next = *plist;
}

// 判断链表是否为空
bool isEmpty(const List list)
{
    return list->next == list;
}

// 向链表的某个节点后插入一个节点
void addNode(List pnode, int item)
{
    List newNode = (List) malloc(sizeof(Node));
    if (newNode == NULL) // 空间分配失败
        exit(1);
    newNode->item = item;
    newNode->next = pnode->next; // 将newNode插入链表内
    pnode->next = newNode;
}

// 删除链表中指定的节点
void delNode(List pnode)
{
    List delNode = pnode->next;
    if (delNode->item == 0) // 如果pnode的后继是头结点，则删除头结点的后继
        pnode = delNode, delNode = delNode->next;
    pnode->next = delNode->next; // 从链表中移除delNode结点
    // printf("%d\n", delNode->item); // 用于代码测试
    free(delNode);
}

// 找到链表中某一节点的后继
List nextNode(const List pnode)
{
    List nItem = pnode->next;
    if (nItem->item == 0) // 如果pnode的后继是头结点，则返回头结点的后继
        nItem = nItem->next;
    return nItem;
}

// 释放链表空间
void destroyList(List * plist)
{
    while (!isEmpty(*plist)) // 如果链表不为空，则删除链表中剩余的节点
        delNode(*plist);
    free(*plist);
    *plist = NULL;
}
