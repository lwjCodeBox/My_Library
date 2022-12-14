#include "LinkedListStack.h"

void  LLS_CreateStack(LinkedListStack **Stack)
{
    //  ������ ���� ����ҿ� ���� 
    (*Stack) = (LinkedListStack *)malloc(sizeof(LinkedListStack));
    (*Stack)->List = NULL;
    (*Stack)->Top = NULL;
}

void LLS_DestroyStack(LinkedListStack *Stack)
{
    while (!LLS_IsEmpty(Stack))
    {
        Node *Popped = LLS_Pop(Stack);
        LLS_DestroyNode(Popped);
    }

    //  ������ ���� ����ҿ��� ���� 
    free(Stack);
}

Node *LLS_CreateNode(char *NewData)
{
    Node *NewNode = (Node *)malloc(sizeof(Node));
    NewNode->Data = (char *)malloc(strlen(NewData) + 1);

    strcpy(NewNode->Data, NewData);  //  �����͸� �����Ѵ�. 

    NewNode->NextNode = NULL; //  ���� ��忡 ���� �����ʹ� NULL�� �ʱ�ȭ�Ѵ�. 

    return NewNode;//  ����� �ּҸ� ��ȯ�Ѵ�. 
}

void  LLS_DestroyNode(Node *_Node)
{
    free(_Node->Data);
    free(_Node);
}

void LLS_Push(LinkedListStack *Stack, Node *NewNode)
{
    if (Stack->List == NULL)
    {
        Stack->List = NewNode;
    }
    else
    {
        //  ������ Top�� �ű� ��带 �����Ѵ�.
        Stack->Top->NextNode = NewNode;
    }

    //  ������ Top �ʵ忡 �� ����� �ּҸ� ����Ѵ�. 
    Stack->Top = NewNode;
    
    printf("Push: %s\n", Stack->Top->Data);
}

Node *LLS_Pop(LinkedListStack *Stack)
{
    //  LLS_Pop() �Լ��� ��ȯ�� �ֻ��� ��� ���� 
    Node *TopNode = Stack->Top;

    if (Stack->List == Stack->Top)
    {
        Stack->List = NULL;
        Stack->Top = NULL;
    }
    else
    {
        // Top �Ʒ��� �ִ� ��带 ���ο� CurrentTop�� ���� 
        Node *CurrentTop = Stack->List;
        while (CurrentTop != NULL && CurrentTop->NextNode != Stack->Top)
        {
            CurrentTop = CurrentTop->NextNode;
        }

        // CurrentTop�� Top�� ����
        Stack->Top = CurrentTop;
        Stack->Top->NextNode = NULL;
    }

    printf("Popped: %s, ", TopNode->Data);

    LLS_DestroyNode(TopNode);

    if (!LLS_IsEmpty(Stack))
        printf("Current Top: %s\n", LLS_Top(Stack)->Data);
    else
        printf("Stack Is Empty.\n");

    return TopNode;
}

Node *LLS_Top(LinkedListStack *Stack)
{
    return Stack->Top;
}

int LLS_GetSize(LinkedListStack *Stack)
{
    int    Count = 0;
    Node *Current = Stack->List;

    while (Current != NULL)
    {
        Current = Current->NextNode;
        Count++;
    }

    return Count;
}

int LLS_IsEmpty(LinkedListStack *Stack)
{
    return (Stack->List == NULL);
}
