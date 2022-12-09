#include "My_LinkedList.h"

Node *SLL_CreateNode(ElementType NewData)
{
    Node *NewNode = (Node *)malloc(sizeof(Node));

    NewNode->Data = NewData;
    NewNode->NextNode = NULL;

    return NewNode;
}



void  SLL_AppendNode(Node **ap_head, Node *ap_new_node)
{
    if((*ap_head) == NULL)
    {
        *ap_head = ap_new_node;
    }
    else{
        Node *Tail = (*ap_head);
        while(Tail->NextNode != NULL){
            Tail = Tail->NextNode;
        }
       Tail->NextNode = ap_new_node; 
    }
}



// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void ShowAllNodeData(Node *ap_list)
{
    Node *next = ap_list;
    for (int i = 0; i < 5; i++){
        printf("%d ", next->Data);
        next = next->NextNode;
    }
}
    