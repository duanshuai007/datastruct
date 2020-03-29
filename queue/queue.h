#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    int write;
    int read;
    bool full;
    int max;
    int itemsize;
} QueueType;

QueueType * InitQueue(int num, int size);
void DeInitQueue(QueueType *p);

bool QueueSend(QueueType *p, void *msg);
bool QueueReceive(QueueType *p, void *msg);

void showQueue(QueueType *p);

#endif
