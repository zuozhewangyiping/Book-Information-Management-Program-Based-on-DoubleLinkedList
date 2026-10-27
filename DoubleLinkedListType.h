#ifndef DOUBLELINKEDLISTTYPE_H
#define DOUBLELINKEDLISTTYPE_H

#include <string.h>
#include <stdlib.h>

#define ISBN_LENGTH 14

typedef struct
{
    char isbn[ISBN_LENGTH];
    char *name;
    double price;
} doublelinkedlist_type;
#define DOUBLELINKEDLIST_TYPE doublelinkedlist_type

#define DOUBELLINKEDLIST_LT(a, b) ((a).price < (b).price)
#define DOUBELLINKEDLIST_GT(a, b) ((a).price > (b).price)
#define DOUBELLINKEDLIST_EQ(a, b) ((a).price == (b).price)
#define DOUBELLINKEDLIST_LE(a, b) ((a).price <= (b).price)
#define DOUBELLINKEDLIST_GE(a, b) ((a).price >= (b).price)

static inline DOUBLELINKEDLIST_TYPE clone_element(const DOUBLELINKEDLIST_TYPE *src, int *judge)
{
    DOUBLELINKEDLIST_TYPE copy_;

    *judge = 1;

    for (int i = 0; i < ISBN_LENGTH; i++)
    {
        copy_.isbn[i] = src->isbn[i];
    }

    copy_.name = NULL;

    if (src->name == NULL)
    {
        *judge = 0;
    }
    else
    {
        int len = strlen(src->name) + 1;

        char *copy = (char *)malloc(len * sizeof(char));
        if (copy == NULL)
        {
            *judge = 0;
        }
        else
        {
            memcpy(copy, src->name, len);

            copy_.name = copy;
        }
    }

    copy_.price = src->price;

    return copy_;
}
#define ELEMENT_CLONE(e, judge) clone_element(&(e), judge)

static inline void destroy_element(DOUBLELINKEDLIST_TYPE *src)
{
    free(src->name);
    src->name = NULL;
}
#define ELEMENT_DESTROY(e) destroy_element(&(e))

#endif