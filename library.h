#ifndef LIBRARY_H
#define LIBRARY_H

/* return
1 if the table is created and printed successfully,
0 if create, read_table or print_table failed */
int task_create(void); // ① 创建并输出

/* return
1 if the table is adjusted and printed successfully,
0 if create, read_table, get_average, price_adjust or print_table failed */
int task_modify(void); // ② 按平均价调价

/* return
1 if the most expensive books are printed successfully,
0 if create, read_table, accumulate_most_expansive or print_most_expansive failed */
int task_most_expensive(void); // ③ 最贵图书

/* return
1 if the book is inserted successfully, or the position is invalid (message printed),
0 if create, read_table, book_number_reading, book_information_reading,
  insert or print_table failed */
int task_insert(void); // ④ 新书入库

/* return
1 if the book is erased successfully, or the position is invalid (message printed),
0 if create, read_table, book_number_reading, erase or print_table failed */
int task_erase(void); // ⑤ 旧书出库

/* return
1 if the table is deduplicated and printed successfully,
0 if create, read_table, deduplicate or print_table failed */
int task_dedup(void); // ⑥ 按 ISBN 去重

/* return
1 if the table is sorted and printed successfully,
0 if create, read_table, book_number_reading, sort or print_table failed */
int task_sort(void); // ⑦ 排序

/* return
1 if the table is reversed and printed successfully,
0 if create, read_table, reverse or print_table failed */
int task_reverse(void); // ⑧ 逆序

#endif
