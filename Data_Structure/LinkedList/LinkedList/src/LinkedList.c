#include "LinkedList.h"

//  ��� ���� 
Node *SLL_CreateNode(ElementType NewData)
{
    Node *NewNode = (Node *)malloc(sizeof(Node));

    NewNode->Data = NewData;  //  �����͸� �����Ѵ�. 
    NewNode->NextNode = NULL; //  ���� ��忡 ���� �����ʹ� NULL�� �ʱ�ȭ �Ѵ�. 

    return NewNode;//  ����� �ּҸ� ��ȯ�Ѵ�. 
}

//  ��� �Ҹ� 
void SLL_DestroyNode(Node *Node)
{
    free(Node);
}

//  ��� �߰� 
void SLL_AppendNode(Node **ap_head, Node *ap_new_node)
{
    //  ��� ��尡 NULL�̶�� ���ο� ��尡 Head 
    if ((*ap_head) == NULL)
    {
        *ap_head = ap_new_node;
    }
    else
    {
        //  ������ ã�� NewNode�� �����Ѵ�. 
        Node *Tail = (*ap_head);
        while (Tail->NextNode != NULL)
        {
            Tail = Tail->NextNode;
        }

        Tail->NextNode = ap_new_node;
    }
}

//  ��� ���� 
void SLL_InsertAfter(Node *ap_current, Node *ap_new_node)
{
    ap_new_node->NextNode = ap_current->NextNode;
    ap_current->NextNode = ap_new_node;
}

void  SLL_InsertNewHead(Node **ap_head, Node *ap_new_node)
{
    if (ap_head == NULL)
    {
        (*ap_head) = ap_new_node;
    }
    else
    {
        ap_new_node->NextNode = (*ap_head);
        (*ap_head) = ap_new_node;
    }
}

//  ��� ���� 
void SLL_RemoveNode(Node **ap_head, Node *ap_remove)
{
    if (*ap_head == ap_remove)
    {
        *ap_head = ap_remove->NextNode;
    }
    else
    {
        Node *Current = *ap_head;
        while (Current != NULL && Current->NextNode != ap_remove)
        {
            Current = Current->NextNode;
        }

        if (Current != NULL)
            Current->NextNode = ap_remove->NextNode;
    }
}

//  ��� Ž�� 
Node *SLL_GetNodeAt(Node *ap_head, int a_location)
{
    Node *pCurrent = ap_head;

    while (pCurrent != NULL && (--a_location) >= 0)
    {
        pCurrent = pCurrent->NextNode;
    }

    return pCurrent;
}

//  ��� �� ���� 
int SLL_GetNodeCount(Node *Head)
{
    int   Count = 0;
    Node *pCurrent = Head;

    while (pCurrent != NULL)
    {
        pCurrent = pCurrent->NextNode;
        Count++;
    }

    return Count;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  ����Ʈ ��� 
void  SLL_ShowAllData(Node *node)
{
    Node *pCurrent = NULL;

    int Count = SLL_GetNodeCount(node);
    for (int i = 0; i < Count; i++)
    {
        pCurrent = SLL_GetNodeAt(node, i);
        printf("[add: 0x%p] List[%d] : %d \n", pCurrent, i, pCurrent->Data);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ��� ��� �ı�
void SLL_DestroyAllNode(Node *node)
{
    Node *pCurrent = NULL;
    int count = SLL_GetNodeCount(node);
    for (int i = 0; i < count; i++)
    {
        pCurrent = SLL_GetNodeAt(node, 0);

        if (pCurrent != NULL)
        {
            printf("Destory [add: 0x%p] List[%d] : %d \n", pCurrent, i, pCurrent->Data);
            SLL_RemoveNode(&node, pCurrent);
            SLL_DestroyNode(pCurrent);
        }
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ��� �߰��� ����
void SSL_InsertNodeData(Node **node, int a_pos, int a_data)
{
    Node *pCurrent = NULL;
    Node *pNewNode = NULL;

    // �Ǿտ� �߰��� �� ���.
    if (--a_pos < 0) 
    {        
        pNewNode = SLL_CreateNode(a_data);
        SLL_InsertNewHead(node, pNewNode);

#if !_DEBUG
        printf("\n---insert node data---\n");
        SLL_ShowAllData(*node);
        printf("----------------------\n\n");
#endif
    }
    else 
    {
        pCurrent = SLL_GetNodeAt(*node, a_pos);
        pNewNode = SLL_CreateNode(a_data);
        SLL_InsertAfter(pCurrent, pNewNode);
    }
}