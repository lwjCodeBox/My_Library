#include "CircularQueue.h"

int main( void )
{
    CircularQueue* Queue;

    CQ_CreateQueue(&Queue, 10);
    
    CQ_Enqueue(Queue, 1);
    CQ_Enqueue(Queue, 2);
    CQ_Enqueue(Queue, 3);
    CQ_Enqueue(Queue, 4);
    
    for (int i = 0; i < 3; i++)
    {
        printf("Dequeue: %d, ", CQ_Dequeue(Queue));
        printf("Front:%d, Rear:%d\n", Queue->Front, Queue->Rear);
    }

    puts("\n--------------------------------");
    int i = 100;
    while (CQ_IsFull(Queue) == 0)
    {
        CQ_Enqueue(Queue, i++);
        printf("Front:%3d, Rear:%3d\n", Queue->Front, Queue->Rear);
    }

    printf("Capacity: %3d, Size: %3d\n\n",
        Queue->Capacity, CQ_GetSize(Queue));

    puts("\n--------------------------------");
    while (CQ_IsEmpty(Queue) == 0)
    {
        printf("Dequeue: %3d, ", CQ_Dequeue(Queue));
        printf("Front:%3d, Rear:%3d\n", Queue->Front, Queue->Rear);
    }


    CQ_DestroyQueue(Queue);

    return 0;
}
