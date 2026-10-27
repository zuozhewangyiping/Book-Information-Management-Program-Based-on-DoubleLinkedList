#ifndef BOOK_IO_H
#define BOOK_IO_H

#include "DoubleLinkedList.h"

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
1 if the number is read successfully,
0 if the input is terminated, alloc failed, number is NULL,
  or the line is not a single integer */
int book_number_reading(int *number);

#endif
