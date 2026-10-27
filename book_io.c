#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book_io.h"
#include "DoubleLinkedList.h"

/* -------------------- Function Declaration -------------------- */

/* return
1 if a line is read successfully,
0 if EOF is encountered (treated the same as "0 0 0"),
-1 if line or line_size is NULL, or alloc failed */
static int read_line(char **line, int *line_size);

/* return
1 if line is the "0 0 0" terminator (spaces and tabs are tolerated),
0 if line is not the terminator (line must not be NULL) */
static int is_terminator(const char *line);

/* return
1 if the line is fragmented successfully,
0 if line or any output pointer is NULL, or the input format is incorrect
  (ISBN is not 13 characters, or the name/price part is empty) */
static int fragment_line(const char *line, int line_size,
                         int *isbn_head, int *isbn_tail,
                         int *name_head, int *name_tail,
                         int *price_head, int *price_tail); // specially designed for separating line

/* return
1 if the fields are parsed into data successfully,
0 if line or data is NULL, the price is invalid, or alloc failed */
static int parse_line(char *line,
                      int isbn_head, int isbn_tail,
                      int name_head, int name_tail,
                      int price_head, int price_tail,
                      DOUBLELINKEDLIST_TYPE *data);

/* return
1 if a book is read successfully,
0 if the input is terminated (by "0 0 0" or EOF),
-1 if data is NULL, alloc failed, or the input format is incorrect */
int book_information_reading(DOUBLELINKEDLIST_TYPE *data);

/* return
1 if the book is printed successfully,
0 if data is NULL */
int book_print(const DOUBLELINKEDLIST_TYPE *data);

/* return
1 if a single number is located successfully,
0 if line or any output pointer is NULL, the line is empty,
  or non-space characters follow the number */
static int number_finding(const char *line, int *number_head, int *number_tail);

/* return
1 if the number is converted successfully,
0 if line or ans is NULL, or the text is not a valid integer */
static int number_extraction(char *line, int number_head, int number_tail, int *ans);

/* return
1 if the number is read successfully,
0 if the input is terminated, alloc failed, number is NULL,
  or the line is not a single integer */
int book_number_reading(int *number);

/* ------------------- Function implementation ------------------- */

static int read_line(char **line, int *line_size)
{
    if (line == NULL || line_size == NULL)
    {
        return -1;
    }

    *line = NULL; // 防御性措施

    int size = 0;
    int capacity = 1;

    char *buf = (char *)malloc(capacity * sizeof(char));
    if (buf == NULL)
    {
        return -1;
    }

    int c;
    while (1)
    {
        c = getchar();

        if (c == EOF) // 无论如何，只要遇到 EOF 就代表本次输入作废，直接等效于遇到了 "0 0 0"
        {
            free(buf);

            return 0;
        }

        if (c == '\n')
        {
            break;
        }

        if (size + 1 >= capacity) // 需要多留一个位置给'\0'
        {
            capacity *= 2;

            char *temp = (char *)realloc(buf, capacity);
            if (temp == NULL)
            {
                free(buf);

                return -1;
            }

            buf = temp;
        }

        buf[size] = (char)c;

        size++;
    }
    buf[size] = '\0';

    *line = buf;
    *line_size = size;

    return 1;
}

static int is_terminator(const char *line)
{
    const char *cursor = line;

    for (int part = 1; part <= 3; part++)
    {
        while (*cursor == ' ' || *cursor == '\t') // 把每个 Part 最前面的空格跳过
        {
            cursor++;
        }

        if (*cursor != '0')
        {
            return 0;
        }

        cursor++;

        if (part < 3) // 前两个 Part 的 '0' 的下一个字符只能是 ' ' 或 '\t'
        {
            if (*cursor != ' ' && *cursor != '\t')
            {
                return 0;
            }
        }
        else // 直接跳过第三个 '0' 之后的所有空格
        {
            while (*cursor == ' ' || *cursor == '\t')
            {
                cursor++;
            }
        }
    }

    if (*cursor != '\0') // 跳过了第三个 '0' 之后，若遇到的字符不是 '\0'，那么不满足结束条件
    {
        return 0;
    }

    return 1;
}

static int fragment_line(const char *line, int line_size,
                         int *isbn_head, int *isbn_tail,
                         int *name_head, int *name_tail,
                         int *price_head, int *price_tail) // specially designed for separating line
{
    if (line == NULL ||
        isbn_head == NULL ||
        isbn_tail == NULL ||
        name_head == NULL ||
        name_tail == NULL ||
        price_head == NULL ||
        price_tail == NULL)
    {
        return 0;
    }

    int cursor;

    // Part 1. 确定 isbn 部分
    cursor = 0;

    while (cursor < line_size && (line[cursor] == ' ' || line[cursor] == '\t')) // ①去除 isbn 部分的前置空格，并确定 isbn_head
    {
        cursor++;
    }
    *isbn_head = cursor;

    while (cursor < line_size && line[cursor] != ' ' && line[cursor] != '\t') // ②找到从前往后第一个空格的下标，并确定其前一个为 isbn_tail
    {
        cursor++;
    }
    *isbn_tail = cursor - 1;

    // Part 2. 确定 price 部分
    cursor = line_size - 1;

    while (cursor > 0 && (line[cursor] == ' ' || line[cursor] == '\t')) // ③去除 price 部分的后置空格，并确定 price_tail
    {
        cursor--;
    }
    *price_tail = cursor;

    while (cursor > 0 && line[cursor] != ' ' && line[cursor] != '\t') // ④找到从后往前第一个空格的下标，并确定其后一个为 price_head
    {
        cursor--;
    }
    *price_head = cursor + 1;

    // Part 3. 确定 name 部分
    cursor = *isbn_tail + 1;

    while (cursor < line_size && (line[cursor] == ' ' || line[cursor] == '\t')) // ⑤去除 name 部分的前置空格，并确定 name_head
    {
        cursor++;
    }
    *name_head = cursor;

    cursor = *price_head - 1;

    while (cursor > 0 && (line[cursor] == ' ' || line[cursor] == '\t')) // ⑥去除 name 部分的后置空格，并确定为 name_tail
    {
        cursor--;
    }
    *name_tail = cursor;

    // Part 4. 判断用户输入是否标准（"0 0 0" 也被认定为不标准）
    if (*isbn_tail - *isbn_head + 1 != 13)
    {
        return 0;
    }

    if (*price_tail < *price_head)
    {
        return 0;
    }

    if (*name_tail < *name_head)
    {
        return 0;
    }

    return 1;
}

static int parse_line(char *line,
                      int isbn_head, int isbn_tail,
                      int name_head, int name_tail,
                      int price_head, int price_tail,
                      DOUBLELINKEDLIST_TYPE *data)
{
    if (line == NULL || data == NULL)
    {
        return 0;
    }

    // Part 1. 解析 isbn 部分
    int isbn_len = isbn_tail - isbn_head + 1;
    for (int i = 0; i < isbn_len; i++)
    {
        data->isbn[i] = line[isbn_head + i];
    }
    data->isbn[isbn_len] = '\0';

    // Part 2. 解析 price 部分
    line[price_tail + 1] = '\0'; // 对 price 后面的内容进行 '\0' 截断，为了 strtod() 正常使用

    char *end = NULL;
    double price = strtod(line + price_head, &end);

    if (end == line + price_head || *end != '\0') // 若用户的输入不规范，那么 strtod() 的解析就会异常，体现于解析中止的位置（纯防御性编程）
    {
        return 0;
    }

    data->price = price;

    // Part 3. 解析 name 部分
    int name_len = name_tail - name_head + 1;
    char *name = (char *)malloc((name_len + 1) * sizeof(char)); // 多一个位置给结尾的 '\0'
    if (name == NULL)
    {
        return 0;
    }

    for (int i = 0; i < name_len; i++)
    {
        name[i] = line[name_head + i];
    }
    name[name_len] = '\0';

    data->name = name;

    return 1;
}

int book_information_reading(DOUBLELINKEDLIST_TYPE *data)
{
    if (data == NULL)
    {
        return -1;
    }

    char *line = NULL;
    int line_size;

    // Step 1. 调用 read_line()，将整行输入读取为字符串
    int judge__read_line = read_line(&line, &line_size);
    if (judge__read_line == 0)
    {
        free(line);

        return 0;
    }
    else if (judge__read_line == -1)
    {
        free(line);

        return -1;
    }

    // Step 2. 调用 is_terminator()，判断输入是否为中止标记
    if (is_terminator(line) == 1)
    {
        free(line);

        return 0;
    }

    // Step 3. 调用 fragment_line()，将字符串分段
    int isbn_head, isbn_tail, name_head, name_tail, price_head, price_tail;

    int judge__fragment_line = fragment_line(line, line_size,
                                             &isbn_head, &isbn_tail,
                                             &name_head, &name_tail,
                                             &price_head, &price_tail);
    if (judge__fragment_line == 0)
    {
        free(line);

        return -1;
    }

    // Step 4. 调用 parse_line()，将字段传入 data
    int judge__parse_line = parse_line(line,
                                       isbn_head, isbn_tail,
                                       name_head, name_tail,
                                       price_head, price_tail,
                                       data);
    if (judge__parse_line == 0)
    {
        free(line);

        return -1;
    }

    free(line); // 本函数中所有 return 之前均统一 free(line)

    return 1;
}

int book_print(const DOUBLELINKEDLIST_TYPE *data)
{
    if (data == NULL)
    {
        return 0;
    }

    printf("%s %s %.2f\n", data->isbn, data->name, data->price);

    return 1;
}

static int number_finding(const char *line, int *number_head, int *number_tail)
{
    if (line == NULL || number_head == NULL || number_tail == NULL)
    {
        return 0;
    }

    int cursor = 0;

    while (line[cursor] == ' ' || line[cursor] == '\t') // 从前往后找第一个非空格类字符，记其位置为 number_head
    {
        cursor++;
    }
    if (line[cursor] == '\0')
    {
        return 0;
    }
    *number_head = cursor;

    while (line[cursor] != ' ' && line[cursor] != '\t' && line[cursor] != '\0') // 从 number_head 一直往后走，直到再次遇到空格类字符，记其前一位置为 number_tail
    {
        cursor++;
    }
    *number_tail = cursor - 1;

    while (line[cursor] != '\0') // 标记完数字后，字符串里不允许再有其他非空格类字符
    {
        if (line[cursor] != ' ' && line[cursor] != '\t')
        {
            return 0;
        }

        cursor++;
    }

    return 1;
}

static int number_extraction(char *line, int number_head, int number_tail, int *ans)
{
    if (line == NULL || ans == NULL)
    {
        return 0;
    }

    line[number_tail + 1] = '\0';

    char *end = NULL;
    int number = strtol(line + number_head, &end, 10);

    if (end == line + number_head || *end != '\0') // 若用户的输入不规范，那么 strtol() 的解析就会异常，体现于解析中止的位置（纯防御性编程）
    {
        return 0;
    }

    *ans = number;

    return 1;
}

int book_number_reading(int *number)
{
    char *line = NULL;
    int line_size;

    // Step 1. 调用 read_line()，将整行输入读取为字符串
    int judge__read_line = read_line(&line, &line_size);
    if (judge__read_line == 0)
    {
        free(line);

        return 0;
    }
    else if (judge__read_line == -1)
    {
        free(line);

        return 0;
    }

    // Step 2. 调用 number_finding()，将数字的位置找出
    int number_head, number_tail;

    int judge__number_finding = number_finding(line, &number_head, &number_tail);
    if (judge__number_finding == 0)
    {
        free(line);

        return 0;
    }

    // Step 3. 调用 number_extraction()，将数字传入 number
    int judge__number_extraction = number_extraction(line, number_head, number_tail, number);
    if (judge__number_extraction == 0)
    {
        free(line);

        return 0;
    }

    free(line);

    return 1;
}
