#pragma once
#include "LinkedList.h"

int main(void)
{
    Node *List = NULL;
    Node *NewNode = NULL;

    puts("+-+-+- Start +-+-+-\n");
    //  ��� 5�� �߰� 
    for (int i = 0; i < 5; i++)
    {
        NewNode = SLL_CreateNode(i);
        SLL_AppendNode(&List, NewNode);
    }

    NewNode = SLL_CreateNode(-1);
    SLL_InsertNewHead(&List, NewNode);

    NewNode = SLL_CreateNode(-2);
    SLL_InsertNewHead(&List, NewNode);

    //  ����Ʈ ��� 
    SLL_ShowAllData(List);

    //  ����Ʈ�� ����° ��� �ڿ� �� ��� ���� 
    printf("\nInserting 3000 After [2]...\n");
    SSL_InsertNodeData(&List, 0, 2222);
    SLL_ShowAllData(List);

    printf("\nInserting 7777 After [4]...\n");
    SSL_InsertNodeData(&List, 4, 4444);
    SLL_ShowAllData(List);

    printf("\nInserting 6666 After [6]...\n");
    SSL_InsertNodeData(&List, 6, 6666);
    SLL_ShowAllData(List);

    //  ��� ��带 �޸𸮿��� ����     
    printf("\nDestroying List...\n");
    SLL_DestroyAllNode(List);
    
    puts("+-+-+- End +-+-+-\n");
    return 0;
}
