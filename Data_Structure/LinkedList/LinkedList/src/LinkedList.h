#pragma once
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

typedef struct tagNode
{
    ElementType Data;
    struct tagNode *NextNode;
} Node;

// 함수 원형 선언 
Node *SLL_CreateNode(ElementType NewData);
void  SLL_DestroyNode(Node *Node);
void  SLL_AppendNode(Node **ap_head, Node *ap_new_node);
void  SLL_InsertAfter(Node *ap_current, Node *ap_new_node);
void  SLL_InsertNewHead(Node **ap_head, Node *ap_new_node);
void  SLL_RemoveNode(Node **ap_head, int a_index);
Node *SLL_GetNodeAt(Node *ap_head, int a_location);
int   SLL_GetNodeCount(Node *Head);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void  SLL_ShowAllData(Node *node);
void  SLL_DestroyAllNode(Node *node);
void  SSL_InsertNodeData(Node **node, int a_pos, int a_data);
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#endif
