#pragma once
#include "LinkedList.h"

int main(void)
{
    Node *List = NULL;
    Node *NewNode = NULL;

    puts("+-+-+- Start +-+-+-\n");
    //  노드 5개 추가 
    for (int i = 0; i < 5; i++)
    {
        NewNode = SLL_CreateNode(i);
        SLL_AppendNode(&List, NewNode);
    }

    NewNode = SLL_CreateNode(-1);
    SLL_InsertNewHead(&List, NewNode);

    NewNode = SLL_CreateNode(-2);
    SLL_InsertNewHead(&List, NewNode);

    //  리스트 출력 
    SLL_ShowAllData(List);

    //  리스트의 세번째 노드 뒤에 새 노드 삽입 
    printf("\nInserting 3000 After [2]...\n");
    SSL_InsertNodeData(&List, 0, 2222);
    SLL_ShowAllData(List);

    printf("\nInserting 7777 After [4]...\n");
    SSL_InsertNodeData(&List, 4, 4444);
    SLL_ShowAllData(List);

    printf("\nInserting 6666 After [6]...\n");
    SSL_InsertNodeData(&List, 6, 6666);
    SLL_ShowAllData(List);

    //  모든 노드를 메모리에서 제거     
    printf("\nDestroying List...\n");
    SLL_DestroyAllNode(List);
    
    puts("+-+-+- End +-+-+-\n");
    return 0;
}
