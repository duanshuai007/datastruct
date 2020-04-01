#include "stack.h"
#include <string.h>

StackType * InitStack(int num, int size)
{
    StackType * pqt = NULL;
    pqt = (StackType *)malloc(sizeof(StackType));
    if (pqt) {
        pqt->depth = 0;
        pqt->max = num;
        pqt->itemsize = size;
        pqt->buffer = (char *)malloc(num * size * sizeof(char));
        pqt->Lock = false;
        if (pqt->buffer == NULL) {
            free(pqt);
            return NULL;
        }
        return pqt;
    }

    return NULL;
}

void DeInitStack(StackType *p)
{
    if (p) {
        if (p->buffer)
            free(p->buffer);
        free(p);
    }
}

bool StackPush(StackType *p, void *msg)
{
    if (p->Lock)
        return false;

    p->Lock = true;

    if (p->depth == p->max)
        return false;

    memcpy(p->buffer + (p->depth * p->itemsize), msg, p->itemsize);

    p->depth += 1;

    p->Lock = false;

    return true;
}

bool StackPop(StackType *p, void *msg)
{
    if (p->Lock)
        return false;
    
    p->Lock = true;

    if (p->depth == 0) {
        printf("the stack is empty\n");
        return false;
    }

    memcpy(msg, p->buffer + ((p->depth - 1)* p->itemsize), p->itemsize);
    p->depth -= 1;

    p->Lock = false;

    return true;
}

void showStack(StackType *p) 
{
    printf("========== start show ==========\n");
    printf("p->depth = %d,  p->max = %d\n", p->depth, p->max);
    for (int i = 0; i < p->max * p->itemsize; i++) {
        if (i != 0 && i % p->itemsize == 0)
            printf("\n");
        printf("%02x ", *(p->buffer + i));
    }
    printf("\n========== end   show ==========\n");
}
