#include "seqlist_ptr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

void InitList(seqlist_t * l)
{
    l->cursize = 0;
    l->maxsize = ListSize;
    memset(l->list, 0, sizeof(DataType *)*ListSize);
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

    return l->list[pos];
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
        //printf("paixu!\n");
        for (j = l->maxsize - 1; j > i; j--) {
            if (j == l->maxsize - 1) {
                if (l->list[j])
                    free(l->list[j]);
            }
            //printf("xianghoupaixu\n");
            l->list[j] = NULL;
            l->list[j] = l->list[j-1];
        }
        //printf("insert!\n");
        l->list[i] = NULL; 
        l->list[i] = e;
        //printf("+1\n");
        l->cursize += 1;
        return 0;
    }
}

int RemoveList(seqlist_t *l, int i)
{
    int j;
    if (i < 0 || i > l->cursize)
        return -1;
    else if (l->cursize <= 0)
        return -2;
    else {
        if (i == (l->maxsize - 1)) {
            //如果是最后一个节点
            if (l->list[i]) {
                free(l->list[i]);
                l->list[i] = NULL;
            }
        } else {
            if (l->list[i]) {
                free(l->list[i]); 
                l->list[i] = NULL;
            }
            for (j = i; j < l->cursize - 1; j++) {
                l->list[j] = l->list[j+1];
            }
            //printf("j = %d\n", j);
            l->list[j] = NULL; //最后一个被移动的指针需要将原位置设置为NULL
        }
        l->cursize--;
        return true;
    }
}
