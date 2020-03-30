#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    int depth;
    int max;
    int itemsize;
    volatile bool Lock;  //锁
} StackType;

//初始化栈空间
//num=栈深度 size=每个单元的大小
StackType * InitStack(int num, int size);
void DeInitStack(StackType *p);

bool StackPush(StackType *p, void *msg);
bool StackPop(StackType *p, void *msg);

void showStack(StackType *p);

#endif
