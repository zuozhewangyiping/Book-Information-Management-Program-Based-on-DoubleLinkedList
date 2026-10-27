#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include "DoubleLinkedListType.h"

typedef struct DoubleLinkedList DoubleLinkedList;
typedef struct DoubleLinkedListNode DoubleLinkedListNode;

/* return
1 if the list is created successfully,
0 if dl is NULL or alloc failed */
int doublelinkedlist_create(DoubleLinkedList **dl);

/* return
none (void) */
void doublelinkedlist_destroy(DoubleLinkedList **dl);

/* return
none (void) */
void doublelinkedlist_clear(DoubleLinkedList *dl);

/* return
the number of books,
-1 if dl is NULL */
int doublelinkedlist_size(const DoubleLinkedList *dl);

/* return
1 if successful,
0 if dl is NULL, the list is empty, or x is NULL */
int doublelinkedlist_peek_front(const DoubleLinkedList *dl, const DOUBLELINKEDLIST_TYPE **x);
int doublelinkedlist_peek_front_mut(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE **x);
int doublelinkedlist_peek_back(const DoubleLinkedList *dl, const DOUBLELINKEDLIST_TYPE **x);
int doublelinkedlist_peek_back_mut(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE **x);

/* return
1 if successful,
0 if dl or x is NULL, the list is empty, or index is out of range */
int doublelinkedlist_get(const DoubleLinkedList *dl, int index, const DOUBLELINKEDLIST_TYPE **x);

/* return
1 if successful,
0 if dl or x is NULL, or index is out of range */
int doublelinkedlist_get_mut(DoubleLinkedList *dl, int index, DOUBLELINKEDLIST_TYPE **x);

/* return
1 if successful,
0 if index is out of range, or cloning failed (alloc failed or name is NULL) */
int doublelinkedlist_set(DoubleLinkedList *dl, int index, DOUBLELINKEDLIST_TYPE new_data);

/* return
1 if successful,
0 if dl is NULL or node cloning failed */
int doublelinkedlist_push_front(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE new_data);
int doublelinkedlist_push_back(DoubleLinkedList *dl, DOUBLELINKEDLIST_TYPE new_data);

/* return
1 if successful,
0 if dl is NULL, index is out of [0, size], or node cloning failed */
int doublelinkedlist_insert(DoubleLinkedList *dl, int index, DOUBLELINKEDLIST_TYPE new_data);

/* return
1 if successful,
0 if dl is NULL or the list is empty */
int doublelinkedlist_pop_front(DoubleLinkedList *dl);
int doublelinkedlist_pop_back(DoubleLinkedList *dl);

/* return
1 if successful,
0 if dl is NULL or index is out of [0, size - 1] */
int doublelinkedlist_erase(DoubleLinkedList *dl, int index);

/* collation == 1: ascending by price;
   any other value: descending by price
return
1 if successful (including lists of size 0 or 1),
0 if dl is NULL */
int doublelinkedlist_sort(DoubleLinkedList *dl, int collation);

/* return
1 if successful (including lists of size 0 or 1),
0 if dl is NULL */
int doublelinkedlist_reverse(DoubleLinkedList *dl);

/* return
1 if successful,
0 if dl or cursor is NULL,
-1 if the list is empty */
int doublelinkedlist_node_begin(const DoubleLinkedList *dl, const DoubleLinkedListNode **cursor);
int doublelinkedlist_node_begin_mut(DoubleLinkedList *dl, DoubleLinkedListNode **cursor);
int doublelinkedlist_node_rbegin(const DoubleLinkedList *dl, const DoubleLinkedListNode **cursor);
int doublelinkedlist_node_rbegin_mut(DoubleLinkedList *dl, DoubleLinkedListNode **cursor);

/* return
1 if the cursor is valid and has been advanced (it may now be NULL),
0 if cursor is NULL or *cursor is NULL */
int doublelinkedlist_node_next(const DoubleLinkedListNode **cursor);
int doublelinkedlist_node_next_mut(DoubleLinkedListNode **cursor);
int doublelinkedlist_node_prev(const DoubleLinkedListNode **cursor);
int doublelinkedlist_node_prev_mut(DoubleLinkedListNode **cursor);

/* return
1 if successful,
0 if cursor or data is NULL */
int doublelinkedlist_node_get_data(const DoubleLinkedListNode *cursor, const DOUBLELINKEDLIST_TYPE **data);
int doublelinkedlist_node_get_data_mut(DoubleLinkedListNode *cursor, DOUBLELINKEDLIST_TYPE **data);

/* victim must belong to dl; *victim is set to NULL after erasing
return
1 if successful,
0 if dl, victim or *victim is NULL */
int doublelinkedlist_node_erase(DoubleLinkedList *dl, DoubleLinkedListNode **victim);

#endif
