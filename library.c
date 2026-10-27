#include <stdio.h>
#include <string.h>
#include "library.h"
#include "book_io.h"
#include "DoubleLinkedList.h"

/* -------------------- Function Declaration -------------------- */

/* return
1 if the whole table is read successfully,
0 if dl is NULL, the input format is incorrect, or push_back failed */
static int read_table(DoubleLinkedList *dl);

/* return
1 if successful (including an empty table),
0 if dl is NULL or cursor access failed */
static int print_table(const DoubleLinkedList *dl);

/* return
1 if the average is computed successfully (0 for an empty table),
0 if dl or average is NULL, or cursor access failed */
static int get_average(const DoubleLinkedList *dl, double *average);

/* return
1 if successful (including an empty table),
0 if dl is NULL or cursor access failed */
static int price_adjust(DoubleLinkedList *dl, double average);

/* return
1 if the most expensive books are counted successfully (0 for an empty table),
0 if dl or amount is NULL, or cursor access failed */
static int accumulate_most_expansive(const DoubleLinkedList *dl, int *amount);

/* return
1 if successful (including an empty table),
0 if dl is NULL or cursor access failed */
static int print_most_expansive(const DoubleLinkedList *dl, int amount);

/* return
1 if successful (including an empty table),
0 if dl is NULL or cursor/erase failed */
static int deduplicate(DoubleLinkedList *dl);

/* ------------------- Function implementation ------------------- */

static int read_table(DoubleLinkedList *dl)
{
    if (dl == NULL)
    {
        return 0;
    }

    while (1)
    {
        DOUBLELINKEDLIST_TYPE data = {0};

        int judge__book_information_reading = book_information_reading(&data);

        if (judge__book_information_reading == 1)
        {
            if (!doublelinkedlist_push_back(dl, data))
            {
                ELEMENT_DESTROY(data);

                return 0;
            }
        }
        else if (judge__book_information_reading == 0) // 遇到中止标记就退出循环
        {
            break;
        }
        else
        {
            return 0;
        }

        ELEMENT_DESTROY(data);
    }

    return 1;
}

static int print_table(const DoubleLinkedList *dl)
{
    if (dl == NULL)
    {
        return 0;
    }

    printf("%d\n", doublelinkedlist_size(dl));

    const DoubleLinkedListNode *cursor = NULL;
    const DOUBLELINKEDLIST_TYPE *data = NULL;

    int judge__doublelinkedlist_node_begin = doublelinkedlist_node_begin(dl, &cursor);

    if (judge__doublelinkedlist_node_begin == 0)
    {
        return 0;
    }
    else if (judge__doublelinkedlist_node_begin == -1)
    {
        return 1;
    }

    while (cursor != NULL)
    {
        if (!doublelinkedlist_node_get_data(cursor, &data))
        {
            return 0;
        }

        book_print(data);
        doublelinkedlist_node_next(&cursor);
    }

    return 1;
}

int task_create(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 打印书单
    if (!print_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 4.正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}

static int get_average(const DoubleLinkedList *dl, double *average)
{
    if (dl == NULL)
    {
        return 0;
    }

    double sum = 0;
    const DoubleLinkedListNode *cursor = NULL;
    const DOUBLELINKEDLIST_TYPE *data = NULL;

    int judge__doublelinkedlist_node_begin = doublelinkedlist_node_begin(dl, &cursor);

    if (judge__doublelinkedlist_node_begin == 0)
    {
        return 0;
    }
    else if (judge__doublelinkedlist_node_begin == -1)
    {
        *average = 0;

        return 1;
    }

    while (cursor != NULL)
    {
        if (!doublelinkedlist_node_get_data(cursor, &data))
        {
            return 0;
        }

        sum += data->price;

        doublelinkedlist_node_next(&cursor);
    }

    int size = doublelinkedlist_size(dl);

    *average = sum / size;

    return 1;
}

static int price_adjust(DoubleLinkedList *dl, double average)
{
    if (dl == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *cursor = NULL;
    DOUBLELINKEDLIST_TYPE *data = NULL;

    int judge__doublelinkedlist_node_begin_mut = doublelinkedlist_node_begin_mut(dl, &cursor);

    if (judge__doublelinkedlist_node_begin_mut == 0)
    {
        return 0;
    }
    else if (judge__doublelinkedlist_node_begin_mut == -1)
    {
        return 1;
    }

    while (cursor != NULL)
    {
        if (!doublelinkedlist_node_get_data_mut(cursor, &data))
        {
            return 0;
        }

        if (data->price < average)
        {
            data->price *= 1.2;
        }
        else
        {
            data->price *= 1.1;
        }

        doublelinkedlist_node_next_mut(&cursor);
    }

    return 1;
}

int task_modify(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 计算书籍价格平均值并打印
    double average = 0;
    if (!get_average(dl, &average))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    printf("%.2f\n", average);

    // Step 4. 根据平均值调整价格
    if (!price_adjust(dl, average))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 5. 打印书单
    if (!print_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 6.正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}

static int accumulate_most_expansive(const DoubleLinkedList *dl, int *amount)
{
    if (dl == NULL || amount == NULL)
    {
        return 0;
    }

    *amount = 0;

    const DoubleLinkedListNode *cursor = NULL;
    const DOUBLELINKEDLIST_TYPE *data = NULL;

    int judge__doublelinkedlist_node_begin = doublelinkedlist_node_begin(dl, &cursor);

    if (judge__doublelinkedlist_node_begin == 0)
    {
        return 0;
    }
    else if (judge__doublelinkedlist_node_begin == -1)
    {
        return 1;
    }

    if (!doublelinkedlist_node_get_data(cursor, &data)) // 先获取最贵书籍的数据
    {
        return 0;
    }

    double most_expansive = data->price; // 记其价格为最贵价格

    while (cursor != NULL)
    {
        if (!doublelinkedlist_node_get_data(cursor, &data))
        {
            return 0;
        }

        if (data->price == most_expansive)
        {
            (*amount)++;
        }
        else
        {
            break;
        }

        doublelinkedlist_node_next(&cursor);
    }

    return 1;
}

static int print_most_expansive(const DoubleLinkedList *dl, int amount)
{
    if (dl == NULL)
    {
        return 0;
    }

    printf("%d\n", amount); // 先打印最贵书籍的数量

    const DoubleLinkedListNode *cursor = NULL;
    const DOUBLELINKEDLIST_TYPE *data = NULL;

    int judge__doublelinkedlist_node_begin = doublelinkedlist_node_begin(dl, &cursor);

    if (judge__doublelinkedlist_node_begin == 0)
    {
        return 0;
    }
    else if (judge__doublelinkedlist_node_begin == -1)
    {
        return 1;
    }

    int i = 0;

    while (cursor != NULL && i < amount)
    {
        if (!doublelinkedlist_node_get_data(cursor, &data))
        {
            return 0;
        }

        book_print(data);

        doublelinkedlist_node_next(&cursor);
        i++;
    }

    return 1;
}

int task_most_expensive(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 根据价格字段将链表进行降序排序
    doublelinkedlist_sort(dl, 0);

    // Step 4. 统计最贵书籍数量
    int amount = 0;

    if (!accumulate_most_expansive(dl, &amount))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 5. 打印最贵书籍数量和它们的信息
    if (!print_most_expansive(dl, amount))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 6.正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}

int task_insert(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 读入待入库书籍位置
    int number = 0;

    if (!book_number_reading(&number))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 4. 读入待入库书籍信息
    DOUBLELINKEDLIST_TYPE data = {0};

    if (book_information_reading(&data) != 1)
    {
        ELEMENT_DESTROY(data);

        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 5. 判断用户输入的位置信息是否合法
    int size = doublelinkedlist_size(dl);

    if (number < 1 || number > size + 1)
    {
        printf("抱歉，入库位置非法!\n");

        ELEMENT_DESTROY(data);

        doublelinkedlist_destroy(&dl);

        return 1; /* 已处理完，用户输入不合法位置不认作程序出错，记为成功 */
    }

    // Step 6. 将新书入库
    if (!doublelinkedlist_insert(dl, number - 1, data))
    {
        ELEMENT_DESTROY(data);

        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 7. 打印新书入库后的书单
    if (!print_table(dl))
    {
        ELEMENT_DESTROY(data);

        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 8. 正确路径收尾
    ELEMENT_DESTROY(data);

    doublelinkedlist_destroy(&dl);

    return 1;
}

int task_erase(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 读入待出库库书籍位置
    int number = 0;

    if (!book_number_reading(&number))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 4. 判断用户输入的位置信息是否合法
    int size = doublelinkedlist_size(dl);

    if (number < 1 || number > size)
    {
        printf("抱歉，出库位置非法!\n");

        doublelinkedlist_destroy(&dl);

        return 1; /* 已处理完，用户输入不合法位置不认作程序出错，记为成功 */
    }

    // Step 5. 将指定位置书籍出库
    if (!doublelinkedlist_erase(dl, number - 1))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 6. 打印指定位置书籍出库后的书单
    if (!print_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 7. 正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}

static int deduplicate(DoubleLinkedList *dl)
{
    if (dl == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *begin = NULL;
    DOUBLELINKEDLIST_TYPE *data_begin = NULL;

    int judge__doublelinkedlist_node_begin_mut = doublelinkedlist_node_begin_mut(dl, &begin);

    if (judge__doublelinkedlist_node_begin_mut == 0)
    {
        return 0;
    }
    else if (judge__doublelinkedlist_node_begin_mut == -1)
    {
        return 1;
    }

    DoubleLinkedListNode *cursor = NULL;
    DOUBLELINKEDLIST_TYPE *data_cursor = NULL;

    while (begin != NULL)
    {
        if (!doublelinkedlist_node_get_data_mut(begin, &data_begin))
        {
            return 0;
        }

        cursor = begin;
        doublelinkedlist_node_next_mut(&cursor);

        while (cursor != NULL)
        {
            if (!doublelinkedlist_node_get_data_mut(cursor, &data_cursor))
            {
                return 0;
            }

            if (strcmp(data_begin->isbn, data_cursor->isbn) == 0)
            {
                DoubleLinkedListNode *victim = cursor;

                doublelinkedlist_node_next_mut(&cursor);

                doublelinkedlist_node_erase(dl, &victim);
            }
            else
            {
                doublelinkedlist_node_next_mut(&cursor);
            }
        }

        doublelinkedlist_node_next_mut(&begin);
    }

    return 1;
}

int task_dedup(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 书单去重
    if (!deduplicate(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 4. 打印去重后的书单
    if (!print_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 5. 正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}

int task_sort(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 读入排序的顺序（1升序，0降序）
    int collation = 0;

    if (!book_number_reading(&collation))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 4. 判断用户输入的排序参数是否合法
    if (collation != 0 && collation != 1)
    {
        printf("排序参数非法，只能输入 1（升序）或 0（降序）\n");

        doublelinkedlist_destroy(&dl);

        return 1;
    }

    // Step 5. 书单排序
    if (!doublelinkedlist_sort(dl, collation))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 6. 打印排序后的书单
    if (!print_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 7. 正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}

int task_reverse(void)
{
    // Step 1. 创建链表
    DoubleLinkedList *dl = NULL;

    if (!doublelinkedlist_create(&dl))
    {
        return 0;
    }

    // Step 2. 读入书单
    if (!read_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 3. 反转书单顺序
    if (!doublelinkedlist_reverse(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 4. 打印反转后的书单
    if (!print_table(dl))
    {
        doublelinkedlist_destroy(&dl);

        return 0;
    }

    // Step 5.正确路径收尾
    doublelinkedlist_destroy(&dl);

    return 1;
}
