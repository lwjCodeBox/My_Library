#include "LinkedList.h"

//  노드 생성 
Node *SLL_CreateNode(ElementType NewData)
{
    Node *NewNode = (Node *)malloc(sizeof(Node));

    NewNode->Data = NewData;  //  데이터를 저장한다. 
    NewNode->NextNode = NULL; //  다음 노드에 대한 포인터는 NULL로 초기화 한다. 

    return NewNode;//  노드의 주소를 반환한다. 
}

//  노드 소멸 
void SLL_DestroyNode(Node *Node)
{
    free(Node);
}

//  노드 추가 
void SLL_AppendNode(Node **ap_head, Node *ap_new_node)
{
    //  헤드 노드가 NULL이라면 새로운 노드가 Head 
    if ((*ap_head) == NULL)
    {
        *ap_head = ap_new_node;
    }
    else
    {
        //  테일을 찾아 NewNode를 연결한다. 
        Node *Tail = (*ap_head);
        while (Tail->NextNode != NULL)
        {
            Tail = Tail->NextNode;
        }

        Tail->NextNode = ap_new_node;
    }
}

//  노드 삽입 
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

//  노드 제거 
void SLL_RemoveNode(Node **ap_head, int a_index)
{
    Node *remove_node = SLL_GetNodeAt(*ap_head, a_index);
    if (*ap_head == remove_node)
    {
        *ap_head = remove_node->NextNode;
    }
    else
    {
        Node *Current = *ap_head;
        while (Current != NULL && Current->NextNode != remove_node)
        {
            Current = Current->NextNode;
        }

        if (Current != NULL)
            Current->NextNode = remove_node->NextNode;
    }

    SLL_DestroyNode(remove_node);
}

//  노드 탐색 
Node *SLL_GetNodeAt(Node *ap_head, int a_location)
{
    Node *pCurrent = ap_head;

    while (pCurrent != NULL && (--a_location) >= 0)
    {
        pCurrent = pCurrent->NextNode;
    }

    return pCurrent;
}

//  노드 수 세기 
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
//  리스트 출력 
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
// 모든 노드 파괴
void SLL_DestroyAllNode(Node *node)
{
    Node *pNext_Node = NULL;
    Node *remove_node = node;

    int i = 0;
    while (remove_node != NULL)
    {
        pNext_Node = remove_node->NextNode;
        printf("Destory [add: 0x%p] List[%d] : %d \n", remove_node, i, remove_node->Data);
        free(remove_node);
        ++i;
        remove_node = pNext_Node;
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 노드 중간에 삽입
void SSL_InsertNodeData(Node **node, int a_pos, int a_data)
{
    Node *pCurrent = NULL;
    Node *pNewNode = NULL;

    // 맨앞에 추가를 한 경우.
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