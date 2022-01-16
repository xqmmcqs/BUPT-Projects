//
// Created by xqmmcqs on 2020/9/30.
//

#ifndef LAB1_LIST_H
#define LAB1_LIST_H

#include <stdbool.h>

typedef struct node
{
    int item;
    struct Node * next;
} Node;

typedef Node * List;

/*
 * 操作：初始化链表
 * 后件：plist指向一个循环链表的头结点
 */
void initList(List * plist);

/*
 * 操作：判断链表是否为空
 * 前件：list是循环链表的头结点
 * 后件：如果该链表为空，返回true，否则返回false
 */
bool isEmpty(const List list);

/*
 * 操作：向链表的某个节点后插入一个节点
 * 前件：pnode是链表中的某一个节点
 * 后件：如果成功，pnode之后添加一个新节点，item属性为传入的第二个参数
 */
void addNode(List pnode, int item);

/*
 * 操作：删除链表中指定的节点
 * 前件：pnode是需要删除的节点的前驱且不是头结点
 * 后件：删除链表中的pnode节点的后继
 */
void delNode(List pnode);

/*
 * 操作：找到链表中某一节点的后继
 * 前件：pnode指向链表中的某一个节点
 * 后件：函数返回pnode的后继，并且跳过头结点
 */
List nextNode(const List pnode);

/*
 * 操作：释放链表空间
 * 前件：plist指向需要释放空间的链表的头结点
 * 后件：释放plist指向链表的空间，plist重置为空指针
 */
void destroyList(List * plist);

#endif //LAB1_LIST_H