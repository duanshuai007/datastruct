#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


#define ListSize    20

typedef struct {
    int age;
    char name[10];
    char city[10];
    bool boy;
} DataType;

typedef struct {
    //DataType (*list)[ListSize]; //数组指针，
    DataType *list[ListSize]; //指针数组，数组的内容为指针
    int cursize;
    int maxsize;
} seqlist_t;

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
                    l->list[j] = NULL;
            }
            //printf("xianghoupaixu\n");
            l->list[j] = l->list[j-1];
        }
        //printf("insert!\n");
        l->list[i] = e;
        //printf("+1\n");
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
        if (i == l->maxsize - 1) {
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
            for (;i < l->cursize; i++) {
                l->list[i] = l->list[i+1];
            }
        }
        l->cursize--;
        return true;
    }
}

void printlist(seqlist_t *l)
{
    int i;
    DataType *pDT = NULL;

    for (i = 0; i < l->cursize; i++) 
    {
        pDT = GetListItem(l, i);
        printf("list[%d]:name=%s city=%s\n", i, pDT->name, pDT->city);
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
            printf("insert %dth item to list, dat=%p\n", i, dat);
            InsertList(&list, i, dat);
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
