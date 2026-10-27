#include <stdio.h>
#include "book_io.h"
#include "library.h"

int main(void)
{
    while (1)
    {
        printf("\n=== 图书信息管理 ===\n");
        printf("1 创建并输出   2 修改（按平均价调价）\n");
        printf("3 最贵图书     4 新书入库\n");
        printf("5 旧书出库     6 图书去重\n");
        printf("7 排序         8 逆序\n");
        printf("0 退出\n");
        printf("请选择：");

        int choice = 0;
        if (book_number_reading(&choice) != 1)
        {
            if (feof(stdin))
            {
                break;
            }

            printf("无效选项\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            printf("请输入书单，以 0 0 0 结束：\n");

            if (!task_create())
                printf("操作失败\n");

            break;
        case 2:
            printf("请输入书单，以 0 0 0 结束：\n");

            if (!task_modify())
                printf("操作失败\n");

            break;
        case 3:
            printf("请输入书单，以 0 0 0 结束：\n");

            if (!task_most_expensive())
                printf("操作失败\n");

            break;
        case 4:
            printf("请先输入书单，以 0 0 0 结束；接下来两行分别输入入库位置与新书信息：\n");

            if (!task_insert())
                printf("操作失败\n");

            break;
        case 5:
            printf("请先输入书单，以 0 0 0 结束；接下来一行输入出库位置：\n");

            if (!task_erase())
                printf("操作失败\n");

            break;
        case 6:
            printf("请输入书单，以 0 0 0 结束：\n");

            if (!task_dedup())
                printf("操作失败\n");

            break;
        case 7:
            printf("请输入书单，以 0 0 0 结束；接下来一行输入排序方式（1 升序 / 0 降序）：\n");

            if (!task_sort())
                printf("操作失败\n");

            break;
        case 8:
            printf("请输入书单，以 0 0 0 结束：\n");

            if (!task_reverse())
                printf("操作失败\n");

            break;
        case 0:
            return 0;
        default:
            printf("无效选项\n");
            break;
        }
    }

    return 0;
}
