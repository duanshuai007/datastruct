#ifndef _SEQLIST_H_
#define _SEQLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ListSize    20

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

void InitList(seqlist_t * l);
bool ListEmpty(seqlist_t *l);
int getListCount(seqlist_t *l);
DataType *GetListItem(seqlist_t *l, int pos);
int InsertList(seqlist_t *l, int i, DataType *e);
int RemoveList(seqlist_t *l, int i);

#endif
