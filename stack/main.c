#include "stack.h"
#include <string.h>

typedef struct {
    int age;
    char name[10];
} msg_t;

int main(void)
{
    StackType * stack = NULL;
    stack = InitStack(3, sizeof(msg_t));
    if (StackPush(stack, &msg) == false)
        printf("stack write falied\n");

    msg_t msg;
    msg.age = 10;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item0", sizeof(msg.name));
   
    msg.age = 11;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item1", sizeof(msg.name));
    if (StackPush(stack, &msg) == false)
        printf("stack write falied\n");

    showStack(stack);
    msg.age = 12;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item2", sizeof(msg.name));
    if (StackPush(stack, &msg) == false)
        printf("stack write falied\n");
    showStack(stack);

    StackPop(stack, &msg);
    printf("msg.age=%d, msg.name=%s\n", msg.age, msg.name);

    msg.age = 13;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item3", sizeof(msg.name));
    if (StackPush(stack, &msg) == false)
        printf("stack write falied\n");
    showStack(stack);

    msg.age = 19;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item4", sizeof(msg.name));
    if (StackPush(stack, &msg) == false)
        printf("stack write falied\n");
    showStack(stack);

    msg.age = 22;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item5", sizeof(msg.name));
    if (StackPush(stack, &msg) == false)
        printf("stack write falied\n");
    showStack(stack);

    msg.age = 33;
    memset(msg.name, 0, sizeof(msg.name));
    memcpy(msg.name, "item6", sizeof(msg.name));
    if ( StackPush(stack, &msg) == false)
        printf("stack write falied\n");
    showStack(stack);

    return 0;
}
