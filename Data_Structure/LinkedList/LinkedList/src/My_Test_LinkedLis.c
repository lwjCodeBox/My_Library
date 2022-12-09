#include "My_LinkedList.h"

int main()
{
    Node *List = NULL;
    Node *NewNode = NULL;

    puts("+-+-+-Start+-+-+-\n");
    //  노드 5개 추가 
    for (int i = 0; i < 5; i++)
    {
        NewNode = SLL_CreateNode(i);
        SLL_AppendNode(&List, NewNode);
    }

    ShowAllNodeData(List);

    return 0;
}