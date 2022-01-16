# 程序风格修正作业

学号：2019211397
姓名：毛子恒

~~（吐槽一下，这三个程序为什么都是三格缩进啊）~~

## 程序：fifo_queue.c

- 问题0：71行存在语法错误，`printf`中多传入了一个参数`value`，应当改为：

    ```C
    printf("pop from front fail\n");
    ```

- 问题1：7、8行类成员的命名重复了类名，应当改为：

    ```C
    int value;
    struct fifo_queue_item* next;
    ```

    13、14行存在类似的问题，应当改为：

    ```C
    struct fifo_queue_item* head;
    struct fifo_queue_item* tail;
    ```

- 问题2：35-50行，`if`多路选择过于复杂，应当改为：

    ```C
    if(queue->queue_head == NULL)
        return 0;
    struct fifo_queue_item* item = queue->queue_head;
    if(item->next_item != NULL)
        queue->queue_head = item->next_item;
    else
    {
        queue->queue_head = NULL;
        queue->queue_tail = NULL;
    }
    *value = item->item_value;
    free(item);
    return 1;
    ```

- 问题3：59行、64行存在神秘数问题，建议添加`#define TEST_CASE_SIZE 10`，并且将59行的`10`改成`TEST_CASE_SIZE`，64行的`11`改成`TEST_CASE_SIZE + 1`。

- 问题4：67、68行有些啰嗦，建议直接删除`success`变量，将68行改为`if(fifo_queue_pop_front(&queue, &value))`。

- 问题5：20行后应当检查内存分配是否成功，添加：

    ```C
    if(item == NULL)
        exit(1);
    ```

## 程序：serialize_json.cpp
- 问题1：12行的实现有些复杂，建议采用迭代器实现：

    ```C++
    for(string::iterator c = s.begin(); c != s.end(); ++c)
    ```

- 问题2：22行的条件不符合直觉，应当改为`if (c < '\x00' || *c > '\x1f')`

- 问题3：37行，否定判断不符合直觉，建议改为`bool is_first = true;`，并且将39-42行改为：

    ```C++
    if(is_first)
        is_first = false;
    else
        o << ",";
    ```

- 问题4：9行，函数命名不明确，建议改为`escape_to_json`。

- 问题5：25行，`switch`的`default`分支也最好加上`break`。

- 问题6：32行，建议给参数加上`const`限定，改为`const map<string, string> &m`。

- 问题7：38行，大括号前加一个空格。

## 程序：split_str.cpp

- 问题1：8行，函数名建议改成函数的功能`split_str`。

- 问题2：47、49、50行以及程序中的其他位置出现神秘数，建议在程序头添加：

    ```C++
    const int STRING_NUMBER = 30;
    const int STRING_LENGTH = 32;
    ```

    47-50行改为：

    ```C++
    char* argv[STRING_NUMBER];
    int i;
    for(i = 0; i < STRING_NUMBER; i++)
    	argv[i] = new char[STRING_LENGTH];
    ```

    其他地方的改动见下文。

- 问题3：14-36行，程序逻辑混乱，重写为：

    ```C++
    for(i = 0; buffer[i]; ++i)
    {
        if(buffer[i] == ' ' || buffer[i] == '\t')
        {
            if(j) // 这是非空白字符后的第一个空白字符
            {
                argv[argc][j] = 0;
                argc++;
                j = 0;
                if(argc >= STRING_NUMBER)
                    break;
            }
        }
        else
        {
            argv[argc][j] = buffer[i];
            i++;
            j++;
            if(j >= STRING_LENGTH)
                break;
        }
    }
    ```

- 问题4：50行`new`的字符串对象没有释放，建议在53行后添加：

    ```C++
    for(i = 0; i < STRING_NUMBER; i++)
    	delete [] argv[i];
    ```

- 问题5：C++中建议使用`<cstdio>`头文件而不是`<stdio.h>`

