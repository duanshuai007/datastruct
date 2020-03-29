#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


#define ListSize    20
//typedef int DataType;

typedef struct {
    int age;
    char name[10];
    char city[10];
    bool boy;
} DataType;

typedef struct {
    DataType list[ListSize];
    int cursize;
    int maxsize;
} seqlist_t;

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
        l->list[i] = *e;
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

void printlist(seqlist_t *l)
{
    int i;
    for (i = 0; i < l->cursize; i++) 
    {
        printf("list[%d]:name=%s city=%s\n", i, l->list[i].name, l->list[i].city);
    }
}

int main(void)
{
    seqlist_t list;
    InitList(&list);

    if (ListEmpty(&list))
        printf("the list is empty!\n");
    else
        printf("the list not empty!\n");


    const int NUM = 30;
    
    //生成随机数据
#if 0
    DataType numarray[NUM];

    for (int i = 0; i < NUM; i++)
        numarray[i] = i;

    int p1, p2, temp;
    srand(time(0));
    for(int i = NUM - 1; i > 0; i--) {
        p1 = i;
        p2 = rand() % i;
        temp = numarray[p1];
        numarray[p1] = numarray[p2];
        numarray[p2] = temp;
    }

#endif
    for (int i = 0; i < NUM; i++) {
        //DataType dat = numarray[i];
        DataType *dat = (DataType *)malloc(sizeof(DataType));
        if (dat) {
            dat->age = i;
            memcpy(dat->name, "duan", 4);
            dat->name[4] = i + 0x30;
            dat->name[5] = '\0';
            memcpy(dat->city, "shen", 4);
            dat->city[4] = i + 0x30;
            dat->city[5] = '\0';
            dat->boy = false;
            InsertList(&list, i, dat);
            free(dat);
        }
    }

    printlist(&list);

    printf("start remove item\n");
    
    RemoveList(&list, 0);
    printf("cursize=%d\n", getListCount(&list));
    RemoveList(&list, 0);
    printf("cursize=%d\n", getListCount(&list));
    RemoveList(&list, 3);
    printf("cursize=%d\n", getListCount(&list));
    RemoveList(&list, 12);
    printf("cursize=%d\n", getListCount(&list));
    RemoveList(&list, 7);
    printf("cursize=%d\n", getListCount(&list));
    RemoveList(&list, 19);
    printf("cursize=%d\n", getListCount(&list));

    printf("printf all item\n");
    printlist(&list);

    return 0;
}
