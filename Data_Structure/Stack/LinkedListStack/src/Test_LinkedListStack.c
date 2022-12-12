#include "LinkedListStack.h"

int main(void)
{
    int Count = 0;
    LinkedListStack *Stack;

    LLS_CreateStack(&Stack);

    LLS_Push(Stack, LLS_CreateNode("abc"));
    printf("Stack: %s\n", LLS_Top(Stack)->Data);

    LLS_Push(Stack, LLS_CreateNode("def"));
    printf("Stack: %s\n", LLS_Top(Stack)->Data);

    LLS_Push(Stack, LLS_CreateNode("efg"));
    printf("Stack: %s\n", LLS_Top(Stack)->Data);

    LLS_Push(Stack, LLS_CreateNode("hij"));
    printf("Stack: %s\n", LLS_Top(Stack)->Data);

    Count = LLS_GetSize(Stack);
    printf("Size: %d, Top: %s\n\n", Count, LLS_Top(Stack)->Data);

    for (int i = 0; i < Count; i++)
    {
        if (LLS_IsEmpty(Stack))
            break;

        LLS_Pop(Stack);       
    }

    LLS_DestroyStack(Stack);

    return 0;
}
