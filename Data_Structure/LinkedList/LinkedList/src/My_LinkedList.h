#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

typedef struct tagNode
{
    ElementType Data;
    Node *NextNode;
} Node;

Node *SLL_CreateNode(ElementType NewData);
void  SLL_DestroyNode(Node *Node);
void  SLL_AppendNode(Node **ap_head, Node *ap_new_node);
void  SLL_InsertAfter(Node *ap_current, Node *ap_new_node);
void  SLL_InsertNewHead(Node **ap_head, Node *ap_new_node);
void  SLL_RemoveNode(Node **ap_head, Node *ap_remove);
Node *SLL_GetNodeAt(Node *ap_head, int a_location);
int   SLL_GetNodeCount(Node *Head);

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void ShowAllNodeData(Node *ap_list);