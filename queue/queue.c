#include "queue.h"
#include <string.h>

QueueType * InitQueue(int num, int size)
{
    QueueType * pqt = NULL;
    pqt = (QueueType *)malloc(sizeof(QueueType));
    if (pqt) {
        pqt->write = 0;
        pqt->read = 0;
        pqt->full = false;
        pqt->max = num;
        pqt->itemsize = size;
        pqt->buffer = (char *)malloc(num * size * sizeof(char));
        if (pqt->buffer == NULL) {
            free(pqt);
            return NULL;
        }
        return pqt;
    }

    return NULL;
}

void DeInitQueue(QueueType *p)
{
    if (p) {
        if (p->buffer)
            free(p->buffer);
        free(p);
    }
}

bool QueueSend(QueueType *p, void *msg)
{
#if 0
    printf("itemsize=%d, sizeof(msg)=%d, sizeof(*msg)=%d\n", p->itemsize, sizeof(msg), sizeof(*msg));
    if (sizeof(*msg) != p->itemsize) {
        printf("send item error!\n");
        return false;
    }
#endif
    if (p->full)
        return false;

    memcpy(p->buffer + ((p->write % p->max) * p->itemsize), msg, p->itemsize);

    p->write += 1;
    p->write %= p->max;

    if (p->write == p->read)
        p->full = true;

    return true;
}

bool QueueReceive(QueueType *p, void *msg)
{
    if ((p->full == false) && (p->read == p->write)) {
        printf("the queue empty\n");
        return false;
    }

    memcpy(msg, p->buffer + ((p->read % p->max)*p->itemsize), p->itemsize);
    p->read += 1;
    p->read %= p->max;
    p->full = false;

    return true;
}

void showQueue(QueueType *p) 
{
    printf("========== start show ==========\n");
    printf("p->write = %d, p->read=%d, p->max = %d p->full=%s\n", p->write, p->read, p->max, p->full == true ? "true":"false");
    for (int i = 0; i < p->max * p->itemsize; i++) {
        if (i != 0 && i % p->itemsize == 0)
            printf("\n");
        printf("%02x ", *(p->buffer + i));
    }
    printf("\n========== end   show ==========\n");
}
