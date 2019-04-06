#include "ccstack.h"
#include <stdlib.h>

int StCreate(PPCC_STACK Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    PCC_STACK tempStack = (PCC_STACK)malloc(sizeof(CC_STACK));
    if (NULL == tempStack)
    {
        return -1;
    }
    *Stack = tempStack;
    (*Stack)->Top = NULL;
    tempStack = NULL;
    return 0;
}

int StDestroy(PPCC_STACK Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (NULL == *Stack)
    {
        return 0;
    }
    PCC_NODE_STACK tempNode = (*Stack)->Top;
    while (NULL != tempNode)
    {
        (*Stack)->Top = (*Stack)->Top->Next;
        free(tempNode);
        tempNode = (*Stack)->Top;
    }
    tempNode = NULL;
    free(*Stack);
    return 0;
}

int StPush(PCC_STACK Stack, int Value)
{
    if (NULL == Stack)
    {
        return -1;
    }
    PCC_NODE_STACK tempNode = (PCC_NODE_STACK)malloc(sizeof(CC_NODE_STACK));
    if (NULL == tempNode)
    {
        return -1;
    }
    tempNode->Key = Value;
    tempNode->Next = Stack->Top;
    Stack->Top = tempNode;
    tempNode = NULL;
    return 0;
}

int StPop(PCC_STACK Stack, int *Value)
{
    if (NULL == Stack || NULL == Value)
    {
        return -1;
    }
    if (Stack->Top == NULL)
    {
        return -1;
    }
    PCC_NODE_STACK tempNode = Stack->Top;
    *Value = tempNode->Key;
    Stack->Top = tempNode->Next;
    tempNode->Next = NULL;
    free(tempNode);
    tempNode = NULL;
    return 0;
}

int StPeek(PCC_STACK Stack, int *Value)
{
    if (NULL == Stack || NULL == Value)
    {
        return -1;
    }
    if (NULL == Stack->Top)
    {
        return -1;
    }
    *Value = Stack->Top->Key;
    return 0;
}

int StIsEmpty(PCC_STACK Stack)
{
    if(NULL == Stack)
    {
        return -1;
    }
    if (NULL == Stack->Top)
    {
        return 1;
    }
    return 0;
}

int StGetCount(PCC_STACK Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    int count = 0;
    PCC_NODE_STACK tempNode = Stack->Top;
    while (NULL != tempNode)
    {
        count++;
        Stack->Top = Stack->Top->Next;
        tempNode = Stack->Top;
    }
    tempNode = NULL;

    return count;
}

int StClear(PCC_STACK Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (NULL == Stack->Top)
    {
        return 0;
    }
    PCC_NODE_STACK tempNode = Stack->Top;
    while (NULL != tempNode)
    {
        Stack->Top = Stack->Top->Next;
        free(tempNode);
        tempNode = Stack->Top;
    }
    tempNode = NULL;
    return 0;
}

int StPushStack(PCC_STACK Stack, PCC_STACK StackToPush)
{
    if (NULL == Stack || NULL == StackToPush)
    {
        return -1;
    }
    int *tempValue = &StackToPush->Top->Key;
    while (StIsEmpty(StackToPush))
    {
        StPop(StackToPush, tempValue);
        StPush(Stack, *tempValue);
    }
    return 0;
}
