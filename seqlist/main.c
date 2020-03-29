#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "seqlist_ptr.h"


void printlist(seqlist_t *l)
{
    int i;
    DataType *p = NULL;
    printf("cursize = %d, maxsize=%d\n", l->cursize, l->maxsize);
    for (i = 0; i < l->cursize; i++)
    {
        p = GetListItem(l, i);
        if (p)
            printf("list[%d]:name=%s city=%s\n", i, p->name, p->city);
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
    DataType *dat = NULL;

    for (int i = 0; i < NUM; i++) {
        //DataType dat = numarray[i];
        dat = (DataType *)malloc(sizeof(DataType));
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
            dat = NULL;
        }
    }


    printlist(&list);

    printf("start remove item\n");

    RemoveList(&list, 19);
    printlist(&list);

    dat = (DataType *)malloc(sizeof(DataType));
    if (dat) {
        dat->age = 100;
        memcpy(dat->name, "huawei", sizeof(dat->name));
        memcpy(dat->city, "xiamen", sizeof(dat->city));
        dat->boy = true;
        InsertList(&list, 5, dat);
    }

    printf("-------------------\n");
    printlist(&list);
    
    printf("-------------------\n");
    RemoveList(&list, 5);
    printlist(&list);
    

    printf("-------------------\n");
    RemoveList(&list, 0);
    RemoveList(&list, 0);
    RemoveList(&list, 0);
    RemoveList(&list, 0);
    RemoveList(&list, 0);
    printlist(&list);
    return 0;
}


