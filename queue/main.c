#include "queue.h"
#include <string.h>

typedef struct {
    int age;
    char name[10];
} msg_t;

int main(void)
{
    msg_t msg;
    msg.age = 10;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "duanshuai", sizeof(msg.name));
   
    int *p = NULL;


    printf("sizeof(*p)=%lu, sizeof(p)=%lu\n", sizeof(*p), sizeof(p));
    QueueType * queue = NULL;
    queue = InitQueue(3, sizeof(msg_t));
    if (QueueSend(queue, &msg) == false)
        printf("queue write falied\n");

    showQueue(queue);

    msg.age = 11;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "dwanghu", sizeof(msg.name));
    if (QueueSend(queue, &msg) == false)
        printf("queue write falied\n");

    showQueue(queue);
    msg.age = 12;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "dwanghu", sizeof(msg.name));
    if (QueueSend(queue, &msg) == false)
        printf("queue write falied\n");
    showQueue(queue);

    QueueReceive(queue, &msg);
    printf("msg.age=%d, msg.name=%s\n", msg.age, msg.name);

    msg.age = 13;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "heiche", sizeof(msg.name));
    if (QueueSend(queue, &msg) == false)
        printf("queue write falied\n");
    showQueue(queue);

    msg.age = 19;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "didial", sizeof(msg.name));
    if (QueueSend(queue, &msg) == false)
        printf("queue write falied\n");
    showQueue(queue);

    msg.age = 22;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "liwang", sizeof(msg.name));
    if (QueueSend(queue, &msg) == false)
        printf("queue write falied\n");
    showQueue(queue);

    msg.age = 33;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "huise", sizeof(msg.name));
    if ( QueueSend(queue, &msg) == false)
        printf("queue write falied\n");
    showQueue(queue);

    return 0;
}
