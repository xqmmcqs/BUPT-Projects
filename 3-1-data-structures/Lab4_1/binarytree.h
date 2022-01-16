//
// Created by xqmmcqs on 2020/11/2.
//

#ifndef LAB4_1_BINARYTREE_H
#define LAB4_1_BINARYTREE_H

typedef struct node
{
    char data;
    struct node * lc, * rc;
    int pos;
} Node;

/*
 * 操作：建二叉树
 * 前件：标准输入流中为一个字符串，代表二叉树
 * 后件：建立一个二叉树，返回指向该二叉树根节点的指针
 */
Node * buildTree();

/*
 * 操作：先序遍历
 * 前件：point指向一个二叉树的根节点
 * 后件：向标准输出中打印二叉树的先序遍历序列
 */
void preOrderTraverse(Node * point);

/*
 * 操作：中序遍历
 * 前件：point指向一个二叉树的根节点
 * 后件：向标准输出中打印二叉树的中序遍历序列
 */
void inOrderTraverse(Node * point);

/*
 * 操作：后序遍历
 * 前件：point指向一个二叉树的根节点
 * 后件：向标准输出中打印二叉树的后序遍历序列
 */
void postOrderTraverse(Node * point);

/*
 * 操作：输出二叉树图形
 * 前件：point指向一个二叉树的根节点
 * 后件：向标准输出中打印二叉树的图形
 */
void printGraph(Node * point);

/*
 * 操作：释放二叉树空间
 * 前件：point指向一个二叉树的根节点
 * 后件：释放该二叉树占用的空间
 */
void destroyTree(Node * point);

#endif //LAB4_1_BINARYTREE_H