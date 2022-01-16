//
// Created by xqmmcqs on 2020/11/2.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "binarytree.h"

int main()
{
    printf("请输入代表二叉树的字符串: ");
    Node * root = buildTree();
    while (true)
    {
        puts("");
        puts("--------------------");
        puts("|** 选择程序功能 **|");
        puts("--------------------");
        puts("(1) 查看二叉树可视化");
        puts("(2) 查看二叉树先序遍历序列");
        puts("(3) 查看二叉树中序遍历序列");
        puts("(4) 查看二叉树后序遍历序列");
        puts("");
        char op[5];
        while (true)
        {
            printf("输入你的选择 [1-4]: ");
            scanf("%3s", op);
            if (strlen(op) > 1 || op[0] < '1' || op[0] > '4')
                puts("输入有误，请重新输入");
            else
                break;
        }
        if (op[0] == '1')
            printGraph(root);
        else if (op[0] == '2')
            preOrderTraverse(root);
        else if (op[0] == '1')
            inOrderTraverse(root);
        else
            postOrderTraverse(root);
        puts("");
        while (true)
        {
            printf("输入c以继续或输入q以退出: ");
            scanf("%3s", op);
            if (strlen(op) > 1 || op[0] != 'c' && op[0] != 'q')
                puts("输入有误，请重新输入");
            else
                break;
        }
        if (op[0] == 'q')
            break;
    }
    destroyTree(root);
    return 0;
}