#include "seqlist.h"
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#endif

void InitList(seqlist_t * l)
{
    l->cursize = 0;
    l->maxsize = ListSize;
}

bool ListEmpty(seqlist_t *l)
{
    if (l->cursize == 0)
        return true;
    else
        return false;
}

int getListCount(seqlist_t *l)
{
    return l->cursize;
}

DataType *GetListItem(seqlist_t *l, int pos)
{
    if (pos < 0 || pos >= l->cursize)
        return NULL;

    return &l->list[pos];
}
//向线性表中插入一个值，则该位置后面的所有元素向后后退一位。
//最后的位置处的值丢失
int InsertList(seqlist_t *l, int i, DataType *e)
{
    int j;

    if (i < 0 || i >= l->maxsize)
        return -1;
    else if (l->cursize >= l->maxsize)
        return -2;
    else {
        for (j = l->maxsize - 1; j > i; j--)
            l->list[j] = l->list[j-1];
        //l->list[i] = *e;
        memcpy(&l->list[i], e, sizeof(DataType));
        l->cursize += 1;
        return 0;
    }
}

int RemoveList(seqlist_t *l, int i)
{
    int j;
    if (i < 0 || i >= l->cursize)
        return -1;
    else if (l->cursize <= 0)
        return -2;
    else {
        for (;i < l->cursize && i < (l->maxsize - 1); i++) {
            l->list[i] = l->list[i+1];
        }
        l->cursize--;

        return true;
    }
}
