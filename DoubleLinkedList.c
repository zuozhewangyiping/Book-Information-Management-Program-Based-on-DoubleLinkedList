#include "DoubleLinkedList.h"
#include <stdlib.h>

struct DoubleLinkedListNode
{
    DOUBLELINKEDLIST_TYPE data;
    struct DoubleLinkedListNode *next;
    struct DoubleLinkedListNode *prev;
};

typedef struct DoubleLinkedList
{
    int size;
    DoubleLinkedListNode *head;
    DoubleLinkedListNode *tail;
} DoubleLinkedList;

int doublelinkedlist_create(DoubleLinkedList **dl)
{
    if (dl == NULL)
    {
        return 0;
    }

    DoubleLinkedList *new_dl = (DoubleLinkedList *)malloc(sizeof(DoubleLinkedList));
    if (new_dl == NULL)
    {
        *dl = NULL;

        return 0;
    }

    new_dl->head = NULL;
    new_dl->tail = NULL;
    new_dl->size = 0;

    *dl = new_dl;

    return 1;
}

void doublelinkedlist_destroy(DoubleLinkedList **dl)
{
    if (dl == NULL || *dl == NULL)
    {
        return;
    }

    DoubleLinkedListNode *cur = (*dl)->head;

    while (cur != NULL)
    {
        DoubleLinkedListNode *victim = cur;
        cur = cur->next;

        ELEMENT_DESTROY(victim->data);
        free(victim);
    }

    free(*dl);
    *dl = NULL;
}

void doublelinkedlist_clear(DoubleLinkedList *dl)
{
    if (dl == NULL)
    {
        return;
    }

    DoubleLinkedListNode *cur = dl->head;

    while (cur != NULL)
    {
        DoubleLinkedListNode *victim = cur;
        cur = cur->next;

        ELEMENT_DESTROY(victim->data);
        free(victim);
    }

    dl->head = NULL;
    dl->tail = NULL;
    dl->size = 0;
}

int doublelinkedlist_size(const DoubleLinkedList *dl)
{
    return (dl != NULL ? dl->size : -1);
}

int doublelinkedlist_peek_front(const DoubleLinkedList *dl, const DOUBLELINKEDLIST_TYPE **x)
{
    if (dl == NULL || dl->head == NULL || x == NULL)
    {
        return 0;
    }

    *x = &(dl->head->data);

    return 1;
}

int doublelinkedlist_peek_front_mut(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE **x)
{
    if (dl == NULL || dl->head == NULL || x == NULL)
    {
        return 0;
    }

    *x = &(dl->head->data);

    return 1;
}

int doublelinkedlist_peek_back(const DoubleLinkedList *dl, const DOUBLELINKEDLIST_TYPE **x)
{
    if (dl == NULL || dl->head == NULL || x == NULL)
    {
        return 0;
    }

    *x = &(dl->tail->data);

    return 1;
}

int doublelinkedlist_peek_back_mut(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE **x)
{
    if (dl == NULL || dl->head == NULL || x == NULL)
    {
        return 0;
    }

    *x = &(dl->tail->data);

    return 1;
}

int doublelinkedlist_get(const DoubleLinkedList *dl, int index, const DOUBLELINKEDLIST_TYPE **x)
{
    if (dl == NULL || dl->head == NULL || x == NULL || index < 0 || index >= dl->size)
    {
        return 0;
    }

    if (index == 0)
    {
        return doublelinkedlist_peek_front(dl, x);
    }
    if (index == dl->size - 1)
    {
        return doublelinkedlist_peek_back(dl, x);
    }

    DoubleLinkedListNode *cur = dl->head;
    for (int i = 0; i < index; i++)
    {
        cur = cur->next;
    }

    *x = &(cur->data);

    return 1;
}

int doublelinkedlist_get_mut(DoubleLinkedList *dl, int index, DOUBLELINKEDLIST_TYPE **x)
{
    if (dl == NULL || x == NULL || index < 0 || index >= dl->size)
    {
        return 0;
    }

    if (index == 0)
    {
        return doublelinkedlist_peek_front_mut(dl, x);
    }
    if (index == dl->size - 1)
    {
        return doublelinkedlist_peek_back_mut(dl, x);
    }

    DoubleLinkedListNode *cur = dl->head;
    for (int i = 0; i < index; i++)
    {
        cur = cur->next;
    }

    *x = &(cur->data);

    return 1;
}

int doublelinkedlist_set(DoubleLinkedList *dl, int index, DOUBLELINKEDLIST_TYPE new_data)
{
    DOUBLELINKEDLIST_TYPE *x = NULL;

    if (!doublelinkedlist_get_mut(dl, index, &x))
    {
        return 0;
    }

    int judge;
    DOUBLELINKEDLIST_TYPE new_data_c = ELEMENT_CLONE(new_data, &judge);
    if (judge == 0)
    {
        ELEMENT_DESTROY(new_data_c);

        return 0;
    }

    ELEMENT_DESTROY(*x);

    *x = new_data_c;

    return 1;
}

static int create_node(DOUBLELINKEDLIST_TYPE new_data, DoubleLinkedListNode **node) // 将 new_data 的深克隆体的所有权交给容器保管，new_data 本体所有权仍在用户手中
{
    DoubleLinkedListNode *new_node = (DoubleLinkedListNode *)malloc(sizeof(DoubleLinkedListNode));
    if (new_node == NULL)
    {
        return 0;
    }

    int judge;
    DOUBLELINKEDLIST_TYPE new_data_c = ELEMENT_CLONE(new_data, &judge);
    if (judge == 0)
    {
        ELEMENT_DESTROY(new_data_c);
        free(new_node);

        return 0;
    }

    new_node->data = new_data_c;
    new_node->next = NULL;
    new_node->prev = NULL;

    *node = new_node;

    return 1;
}

int doublelinkedlist_push_front(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE new_data)
{
    if (dl == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *new_node = NULL;
    if (!create_node(new_data, &new_node))
    {
        return 0;
    }

    if (dl->head == NULL)
    {
        dl->head = new_node;
        dl->tail = new_node;

        dl->size++;

        return 1;
    }

    new_node->next = dl->head;
    dl->head->prev = new_node;

    dl->head = new_node;

    dl->size++;

    return 1;
}

int doublelinkedlist_push_back(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE new_data)
{
    if (dl == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *new_node = NULL;
    if (!create_node(new_data, &new_node))
    {
        return 0;
    }

    if (dl->head == NULL)
    {
        dl->head = new_node;
        dl->tail = new_node;

        dl->size++;

        return 1;
    }

    new_node->prev = dl->tail;
    dl->tail->next = new_node;

    dl->tail = new_node;

    dl->size++;

    return 1;
}

int doublelinkedlist_insert(DoubleLinkedList *dl, int index, DOUBLELINKEDLIST_TYPE new_data)
{
    if (dl == NULL || index < 0 || index > dl->size)
    {
        return 0;
    }

    if (index == 0)
    {
        return doublelinkedlist_push_front(dl, new_data);
    }
    if (index == dl->size)
    {
        return doublelinkedlist_push_back(dl, new_data);
    }

    // 接下来已保证 0 < index < size

    DoubleLinkedListNode *new_node = NULL;
    if (!create_node(new_data, &new_node))
    {
        return 0;
    }

    DoubleLinkedListNode *cur = dl->head;
    for (int i = 0; i < index - 1; i++)
    {
        cur = cur->next;
    }

    DoubleLinkedListNode *next = cur->next;

    cur->next = new_node;
    new_node->prev = cur;

    new_node->next = next;
    next->prev = new_node;

    dl->size++;

    return 1;
}

int doublelinkedlist_pop_front(DoubleLinkedList *dl)
{
    if (dl == NULL || dl->head == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *victim = dl->head;
    DoubleLinkedListNode *next = victim->next;

    ELEMENT_DESTROY(victim->data);
    free(victim);

    if (next == NULL)
    {
        dl->tail = NULL;
    }
    else
    {
        next->prev = NULL;
    }

    dl->head = next;

    dl->size--;

    return 1;
}

int doublelinkedlist_pop_back(DoubleLinkedList *dl)
{
    if (dl == NULL || dl->head == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *victim = dl->tail;
    DoubleLinkedListNode *prev = victim->prev;

    ELEMENT_DESTROY(victim->data);
    free(victim);

    if (prev == NULL)
    {
        dl->head = NULL;
    }
    else
    {
        prev->next = NULL;
    }

    dl->tail = prev;

    dl->size--;

    return 1;
}

int doublelinkedlist_erase(DoubleLinkedList *dl, int index)
{
    if (dl == NULL || index < 0 || index >= dl->size)
    {
        return 0;
    }

    if (index == 0)
    {
        return doublelinkedlist_pop_front(dl);
    }
    if (index == dl->size - 1)
    {
        return doublelinkedlist_pop_back(dl);
    }

    // 接下来已保证 0 < index < size - 1

    DoubleLinkedListNode *cur = dl->head;
    for (int i = 0; i < index; i++)
    {
        cur = cur->next;
    }

    DoubleLinkedListNode *victim = cur;
    DoubleLinkedListNode *prev = victim->prev;
    DoubleLinkedListNode *next = victim->next;

    ELEMENT_DESTROY(victim->data);
    free(victim);

    prev->next = next;
    next->prev = prev;

    dl->size--;

    return 1;
}

static int merge(DoubleLinkedListNode *left,
                 DoubleLinkedListNode *right,
                 int collation,
                 DoubleLinkedListNode **merged_head,
                 DoubleLinkedListNode **merged_tail) // collation == 1 升序，否则降序；利用 merged_head/merged_tail 将合体链的首位传出
{
    if (left == NULL && right == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode sentinel; // 哨兵节点，作为新链开头，函数结束即销毁
    sentinel.prev = NULL;
    sentinel.next = NULL;

    DoubleLinkedListNode *cur = &sentinel;

    while (left != NULL && right != NULL)
    {
        // 以下命题式为真时 left 应该连上新链，否则 right 应该连上新链（由真值表可得）
        int judge = (collation == 1 ? DOUBELLINKEDLIST_LE(left->data, right->data) : DOUBELLINKEDLIST_GE(left->data, right->data));

        DoubleLinkedListNode *temp; // 用来指向下一个连上新链的节点

        if (judge)
        {
            temp = left;
            left = left->next;
        }
        else
        {
            temp = right;
            right = right->next;
        }

        cur->next = temp;
        temp->prev = cur;

        cur = temp;
    }

    if (left != NULL)
    {
        cur->next = left;
        left->prev = cur;
    }
    else
    {
        cur->next = right;
        right->prev = cur;
    }

    while (cur->next != NULL)
    {
        cur = cur->next;
    }

    sentinel.next->prev = NULL; // 将实际新链与哨兵节点断连

    *merged_head = sentinel.next;
    *merged_tail = cur;

    return 1;
}

int doublelinkedlist_sort(DoubleLinkedList *dl, int collation)
{
    if (dl == NULL)
    {
        return 0;
    }

    if (dl->size <= 1)
    {
        return 1;
    }

    // 用于将每一轮重组后的新链的头和尾传出（head 不置 NULL 是为了方便第一轮时 cur 可直接使用）
    DoubleLinkedListNode *head = dl->head;
    DoubleLinkedListNode *tail = NULL;

    for (int width = 1; width < dl->size; width *= 2)
    {
        DoubleLinkedListNode *cur = head;
        DoubleLinkedListNode *new_head = NULL; // 本轮重组后的新链头
        DoubleLinkedListNode *new_tail = NULL; // 本轮重组后的新链尾

        while (cur != NULL)
        {
            // 第一步：从原链上找出左链和右链各自的头和尾
            DoubleLinkedListNode *left_head = cur;
            DoubleLinkedListNode *left_tail = left_head;
            int left_len;
            for (left_len = 1; left_len < width && left_tail->next != NULL; left_len++)
            {
                left_tail = left_tail->next;
            }

            DoubleLinkedListNode *right_head = left_tail->next;
            DoubleLinkedListNode *right_tail = right_head;
            int right_len;
            if (right_head == NULL)
            {
                right_len = 0;
            }
            else
            {
                for (right_len = 1; right_len < width && right_tail->next != NULL; right_len++)
                {
                    right_tail = right_tail->next;
                }
            }

            // 第二步：及时更新下一次 while 所需的 cur
            cur = (right_tail != NULL ? right_tail->next : NULL);

            // 第三步：将找出的左链和右链从原链上截出
            left_head->prev = NULL;
            left_tail->next = NULL;
            if (right_len > 0)
            {
                right_head->prev = NULL;
                right_tail->next = NULL;
            }

            // 第四步：将截出的左链和右链送入 merge 函数进行合体，得到合体链的头和尾
            DoubleLinkedListNode *merged_head = NULL;
            DoubleLinkedListNode *merged_tail = NULL;

            merge(left_head, right_head, collation, &merged_head, &merged_tail); // 不用检查 merge 返回值，因为不可能失败（返回值设计纯防御性编程）

            // 第五步：将合体链接入新链
            if (new_head == NULL)
            {
                new_head = merged_head;
            }
            else
            {
                new_tail->next = merged_head;
                merged_head->prev = new_tail;
            }
            new_tail = merged_tail;
        }

        // 每一轮结束时将重组的新链的头和尾传出
        head = new_head;
        tail = new_tail;
    }

    // 最后将链表的头和尾进行更新
    dl->head = head;
    dl->tail = tail;

    return 1;
}

int doublelinkedlist_reverse(DoubleLinkedList *dl)
{
    if (dl == NULL)
    {
        return 0;
    }

    if (dl->size <= 1)
    {
        return 1;
    }

    DoubleLinkedListNode *cur = dl->head;

    while (cur != NULL)
    {
        DoubleLinkedListNode *temp = cur;
        cur = temp->next;

        DoubleLinkedListNode *t = temp->prev;
        temp->prev = temp->next;
        temp->next = t;
    }

    DoubleLinkedListNode *temp = dl->head;
    dl->head = dl->tail;
    dl->tail = temp;

    return 1;
}

int doublelinkedlist_node_begin(const DoubleLinkedList *dl, const DoubleLinkedListNode **cursor)
{
    if (dl == NULL || cursor == NULL)
    {
        return 0;
    }

    if (dl->head == NULL)
    {
        return -1;
    }

    *cursor = dl->head;

    return 1;
}

int doublelinkedlist_node_begin_mut(DoubleLinkedList *dl, DoubleLinkedListNode **cursor)
{
    if (dl == NULL || cursor == NULL)
    {
        return 0;
    }

    if (dl->head == NULL)
    {
        return -1;
    }

    *cursor = dl->head;

    return 1;
}

int doublelinkedlist_node_rbegin(const DoubleLinkedList *dl, const DoubleLinkedListNode **cursor)
{
    if (dl == NULL || cursor == NULL)
    {
        return 0;
    }

    if (dl->head == NULL)
    {
        return -1;
    }

    *cursor = dl->tail;

    return 1;
}

int doublelinkedlist_node_rbegin_mut(DoubleLinkedList *dl, DoubleLinkedListNode **cursor)
{
    if (dl == NULL || cursor == NULL)
    {
        return 0;
    }

    if (dl->head == NULL)
    {
        return -1;
    }

    *cursor = dl->tail;

    return 1;
}

int doublelinkedlist_node_next(const DoubleLinkedListNode **cursor)
{
    if (cursor == NULL || *cursor == NULL)
    {
        return 0;
    }

    *cursor = (*cursor)->next;

    return 1;
}

int doublelinkedlist_node_next_mut(DoubleLinkedListNode **cursor)
{
    if (cursor == NULL || *cursor == NULL)
    {
        return 0;
    }

    *cursor = (*cursor)->next;

    return 1;
}

int doublelinkedlist_node_prev(const DoubleLinkedListNode **cursor)
{
    if (cursor == NULL || *cursor == NULL)
    {
        return 0;
    }

    *cursor = (*cursor)->prev;

    return 1;
}

int doublelinkedlist_node_prev_mut(DoubleLinkedListNode **cursor)
{
    if (cursor == NULL || *cursor == NULL)
    {
        return 0;
    }

    *cursor = (*cursor)->prev;

    return 1;
}

int doublelinkedlist_node_get_data(const DoubleLinkedListNode *cursor, const DOUBLELINKEDLIST_TYPE **data)
{
    if (cursor == NULL || data == NULL)
    {
        return 0;
    }

    *data = &(cursor->data);

    return 1;
}

int doublelinkedlist_node_get_data_mut(DoubleLinkedListNode *cursor, DOUBLELINKEDLIST_TYPE **data)
{
    if (cursor == NULL || data == NULL)
    {
        return 0;
    }

    *data = &(cursor->data);

    return 1;
}

int doublelinkedlist_node_erase(DoubleLinkedList *dl, DoubleLinkedListNode **victim)
{
    if (dl == NULL || victim == NULL || *victim == NULL)
    {
        return 0;
    }

    DoubleLinkedListNode *prev = (*victim)->prev;
    if (prev == NULL)
    {
        int judge = doublelinkedlist_pop_front(dl);

        if (!judge)
        {
            return 0;
        }

        *victim = NULL;

        return 1;
    }

    DoubleLinkedListNode *next = (*victim)->next;
    if (next == NULL)
    {
        int judge = doublelinkedlist_pop_back(dl);

        if (!judge)
        {
            return 0;
        }

        *victim = NULL;

        return 1;
    }

    prev->next = next;
    next->prev = prev;

    ELEMENT_DESTROY((*victim)->data);
    free(*victim);

    *victim = NULL;

    dl->size--;

    return 1;
}
