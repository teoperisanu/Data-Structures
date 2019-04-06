#include "cctree.h"
#include <stdlib.h>
#include "ccstack.h"


static int TreeRecomputeHeight(PCC_NODE_TREE Node);
static int TreeBalanceFactor(PCC_NODE_TREE Node);
static PCC_NODE_TREE TreeDoubleRightRotation(PCC_NODE_TREE Node);
static PCC_NODE_TREE TreeDoubleLeftRotation(PCC_NODE_TREE Node);
static PCC_NODE_TREE TreeSingleRightRotation(PCC_NODE_TREE Node);
static PCC_NODE_TREE TreeSingleLeftRotation(PCC_NODE_TREE Node);
static int TreeMinimumKey(PCC_NODE_TREE Node, int *Value);

static PCC_NODE_TREE TreeCreateNode(int Value)
{
    PCC_NODE_TREE node = (PCC_NODE_TREE)malloc(sizeof(CC_NODE_TREE));
    if (NULL == node)
    {
        return node;
    }
    node->Key = Value;
    node->Left = NULL;
    node->Right = NULL;
    node->Height = 0;
    node->Count = 1;
    return node;
}

static int TreeDestroyNode(PPCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return -1;
    }
    if (NULL == (*Node))
    {
        return 0;
    }
    (*Node)->Key = 0;
    (*Node)->Height = 0;
    (*Node)->Count = 0;
    (*Node)->Left = NULL;
    (*Node)->Right = NULL;
    free((*Node));
    (*Node) = NULL;
    return 0;
}

int TreeCreate(PPCC_TREE Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }
    if (NULL != *Tree)
    {
        free(*Tree);
    }
    PCC_TREE tempTree = (PCC_TREE)malloc(sizeof(CC_TREE));
    if (NULL == tempTree)
    {
        return -1;
    }
    (*Tree) = tempTree;
    (*Tree)->Root = NULL;
    (*Tree)->NumberOfNodes = 0;
    tempTree = NULL;
    return 0;
}

int TreeDestroy(PPCC_TREE Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }
    if (NULL == (*Tree))
    {
        return 0;
    }

    if (NULL != (*Tree)->Root)
    {
        if (0 != TreeClear(*Tree))
        {
            return -1;
        }
        (*Tree)->Root = NULL;
    }
    free(*Tree);
    *Tree = NULL;
    return 0;
}

static PCC_NODE_TREE TreeInsertNode(PCC_NODE_TREE Node, int Value)
{
    if (NULL == Node)
    {
        return TreeCreateNode(Value);
    }
    if (Value < Node->Key)
    {
        Node->Left = TreeInsertNode(Node->Left, Value);
    }
    else
    {
        if (Value > Node->Key)
        {
            Node->Right = TreeInsertNode(Node->Right, Value);
        }
        else
        {
            Node->Count++;
        }
    }
    TreeRecomputeHeight(Node);
    int balance = TreeBalanceFactor(Node);
    if (-2 == balance)
    {
        if (TreeBalanceFactor(Node->Right) < 0)
        {
            Node = TreeSingleLeftRotation(Node);
        }
        else
        {
            Node = TreeDoubleLeftRotation(Node);
        }
    }
    else
    {
        if (2 == balance)
        {
            if (TreeBalanceFactor(Node->Left) > 0)
            {
                Node = TreeSingleRightRotation(Node);
            }
            else
            {
                Node = TreeDoubleRightRotation(Node);
            }
        }
    }
    return Node;
}

int TreeInsert(PCC_TREE Tree, int Value)
{
    if (NULL == Tree)
    {
        return -1;
    }
    PCC_NODE_TREE tempNode = TreeInsertNode(Tree->Root, Value);
    if (NULL == tempNode)
    {
        return -1;
    }
    Tree->Root = tempNode;
    Tree->NumberOfNodes++;
    tempNode = NULL;
    return 0;
}

static PCC_NODE_TREE TreeRemoveNode(PCC_NODE_TREE Node, int Value)
{
    if (NULL == Node)
    {
        return Node;
    }
    else
    {
        if (Node->Key > Value)
        {
            Node->Left = TreeRemoveNode(Node->Left, Value);
            //return Node;
        }
        else
        {
            if (Node->Key < Value)
            {
                Node->Right = TreeRemoveNode(Node->Right, Value);
                //return Node;
            }
            else
            {
                if ((NULL == Node->Left) && (NULL == Node->Right))
                {
                    //return NULL;
                    if (0 != TreeDestroyNode(&Node))
                    {
                        return NULL;
                    }

                }
                else
                {
                    if (NULL == Node->Left)
                    {
                        PCC_NODE_TREE tempNode = Node->Right;
                        if (0 != TreeDestroyNode(&Node))
                        {
                            return NULL;
                        }
                        return tempNode;
                    }
                    else
                    {
                        if (NULL == Node->Right)
                        {
                            PCC_NODE_TREE tempNode = Node->Left;
                            if (0 != TreeDestroyNode(&Node))
                            {
                                return NULL;
                            }
                            return tempNode;
                        }
                        else
                        {
                            int min = 0;
                            if (0 != TreeMinimumKey(Node->Right, &min))
                            {
                                return NULL;
                            }
                            Node->Key = min;
                            Node->Right = TreeRemoveNode(Node->Right, min);
                            //return Node;
                        }
                    }
                }
            }
        }
    }
    TreeRecomputeHeight(Node);
    int balance = TreeBalanceFactor(Node);
    if (-2 == balance)
    {
        if (TreeBalanceFactor(Node->Right) < 0)
        {
            Node = TreeSingleLeftRotation(Node);
        }
        else
        {
            Node = TreeDoubleLeftRotation(Node);
        }
    }
    else
    {
        if (2 == balance)
        {
            if (TreeBalanceFactor(Node->Left) > 0)
            {
                Node = TreeSingleRightRotation(Node);
            }
            else
            {
                Node = TreeDoubleRightRotation(Node);
            }
        }
    }
    return Node;
}

int TreeRemove(PCC_TREE Tree, int Value)
{
    if (NULL == Tree)
    {
        return -1;
    }
    PCC_NODE_TREE tempNode = TreeRemoveNode(Tree->Root, Value);

    Tree->Root = tempNode;
    Tree->NumberOfNodes--;
    tempNode = NULL;
    return 0;
}

int TreeContains(PCC_TREE Tree, int Value)
{
    if (NULL == Tree)
    {
        return -1;
    }
    PCC_NODE_TREE currentNode = Tree->Root;
    while (NULL != currentNode)
    {
        if (currentNode->Key == Value)
        {
            return 1;
        }
        if (currentNode->Key > Value)
        {
            currentNode = currentNode->Left;
        }
        else
        {
            currentNode = currentNode->Right;
        }
    }
    return 0;
}

int TreeGetCount(PCC_TREE Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }
    return Tree->NumberOfNodes;
}

int TreeGetHeight(PCC_TREE Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }
    if (NULL == Tree->Root)
    {
        return -1;
    }
    return Tree->Root->Height;
}


int TreeClear(PCC_TREE Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }
    PCC_STACK auxStack = NULL;
    if (0 != StCreate(&auxStack))
    {
        return -1;
    }
    PCC_NODE_TREE currentNode = Tree->Root;
    while (NULL != currentNode)
    {
        if (NULL != currentNode->Right)
        {
            StPush(auxStack, (int)currentNode->Right);
        }
        StPush(auxStack, (int)currentNode);
        currentNode = currentNode->Left;
    }

    while (!StIsEmpty(auxStack))
    {

        int p = 0;
        StPop(auxStack, &p);
        currentNode = (PCC_NODE_TREE)p;



        StPeek(auxStack, &p);
        PCC_NODE_TREE nodeRight = (PCC_NODE_TREE)p;

        if ((NULL != currentNode->Right) && (!StIsEmpty(auxStack)) && (currentNode->Right == nodeRight))
        {
            StPop(auxStack, &p);
            StPush(auxStack, (int)currentNode);
            currentNode = currentNode->Right;
            StPeek(auxStack, &p);
            nodeRight = (PCC_NODE_TREE)p;
        }
        else
        {
            if (0 != TreeDestroyNode(&currentNode))
            {
                return -1;
            }
            currentNode = NULL;
        }
        while (NULL != currentNode)
        {
            if (NULL != currentNode->Right)
            {
                StPush(auxStack, (int)currentNode->Right);
            }
            StPush(auxStack, (int)currentNode);
            currentNode = currentNode->Left;
        }
    }
    StDestroy(&auxStack);
    currentNode = NULL;
    Tree->Root = NULL;
    Tree->NumberOfNodes = 0;
    return 0;
}



int TreeGetNthPreorder(PCC_TREE Tree, int Index, int *Value)
{
    if (NULL == Tree || NULL == Value)
    {
        return -1;
    }
    if (Index >= Tree->NumberOfNodes || Index < 0)
    {
        return -1;
    }
    PCC_STACK auxStack = NULL;
    if (0 != StCreate(&auxStack))
    {
        return -1;
    }
    PCC_NODE_TREE currentNode = Tree->Root;
    if (NULL != Tree->Root)
    {
        StPush(auxStack, (int)Tree->Root);
    }
    int count = -1;

    
    while (!StIsEmpty(auxStack))
    {
        
        int p = 0;
        StPop(auxStack, &p);
        currentNode = (PCC_NODE_TREE)p;

        count += currentNode->Count;
        
        if ((Index <= count) && (Index >= (count - currentNode->Count)))
        {
            *Value = currentNode->Key;
            break;
        }
        if (NULL != currentNode->Right)
        {
            StPush(auxStack, (int)currentNode->Right);
        }
        if (NULL != currentNode->Left)
        {
            StPush(auxStack, (int)currentNode->Left);
        }
    }
    StDestroy(&auxStack);
    currentNode = NULL;
    return 0;
}

int TreeGetNthInorder(PCC_TREE Tree, int Index, int *Value)
{
    if (NULL == Tree || NULL == Value)
    {
        return -1;
    }
    if (Index >= Tree->NumberOfNodes || Index < 0)
    {
        return -1;
    }
    PCC_STACK auxStack = NULL;
    if (0 != StCreate(&auxStack))
    {
        return -1;
    }
    PCC_NODE_TREE currentNode = Tree->Root;
    while (NULL != currentNode)
    {
        StPush(auxStack, (int)currentNode);
        currentNode = currentNode->Left;
    }

    int count = -1;
    while (!StIsEmpty(auxStack))
    {

        int p = 0;
        StPop(auxStack, &p);
        currentNode = (PCC_NODE_TREE)p;

        count += currentNode->Count;

        if ((Index <= count) && (Index >= (count - currentNode->Count)))
        {
            *Value = currentNode->Key;
            break;
        }

        currentNode = currentNode->Right;
        while (NULL != currentNode)
        {
            StPush(auxStack, (int)currentNode);
            currentNode = currentNode->Left;
        }
       
    }
    StDestroy(&auxStack);
    currentNode = NULL;
    return 0;
}

int TreeGetNthPostorder(PCC_TREE Tree, int Index, int *Value)
{
    if (NULL == Tree || NULL == Value)
    {
        return -1;
    }
    if (Index >= Tree->NumberOfNodes || Index < 0)
    {
        return -1;
    }
    PCC_STACK auxStack = NULL;
    if (0 != StCreate(&auxStack))
    {
        return -1;
    }
    PCC_NODE_TREE currentNode = Tree->Root;
    while (NULL != currentNode)
    {
        if (NULL != currentNode->Right)
        {
            StPush(auxStack, (int)currentNode->Right);
        }
        StPush(auxStack, (int)currentNode);
        currentNode = currentNode->Left;
    }

    int count = -1;
    while (!StIsEmpty(auxStack))
    {

        int p = 0;
        StPop(auxStack, &p);
        currentNode = (PCC_NODE_TREE)p;

        

        StPeek(auxStack, &p);
        PCC_NODE_TREE nodeRight = (PCC_NODE_TREE)p;

        if ((NULL != currentNode->Right) && (!StIsEmpty(auxStack)) && (currentNode->Right == nodeRight))
        {
            StPop(auxStack, &p);
            StPush(auxStack, (int)currentNode);
            currentNode = currentNode->Right;
            StPeek(auxStack, &p);
            nodeRight = (PCC_NODE_TREE)p;
        }
        else
        {
            count += currentNode->Count;
            if ((Index <= count) && (Index >= (count - currentNode->Count)))
            {
                *Value = currentNode->Key;
                break;
            }
            currentNode = NULL;
        }
        while (NULL != currentNode)
        {
            if (NULL != currentNode->Right)
            {
                StPush(auxStack, (int)currentNode->Right);
            }
            StPush(auxStack, (int)currentNode);
            currentNode = currentNode->Left;
        } 
    }
    StDestroy(&auxStack);
    currentNode = NULL;
    return 0;
}

static int TreeMaxHeightTwoNodes(int HeightFirstNode, int HeightSecondNode);

/**
 *Recomputes the height of a given Node
 *
 *@param Node: a Node of the Tree
 *@return: -1 if an error occurs, otherwise 0
 */
static int TreeRecomputeHeight(PCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return -1;
    }
    if ((NULL == Node->Left) && (NULL == Node->Right))
    {
        Node->Height = 0;
    }
    else 
    {
        if (NULL == Node->Left)
        {
            Node->Height = Node->Right->Height + 1;
        }
        else
        {
            if (NULL == Node->Right)
            {
                Node->Height = Node->Left->Height + 1;
            }
            else
            {
                Node->Height = TreeMaxHeightTwoNodes(Node->Left->Height, Node->Right->Height) + 1;
            }
        }
    }
    return 0;
}

/**
 *Calculates the balance factor of the Node
 *
 *@param Node: a Node of the Tree
 *@return: -1 if an error occurs, otherwise the balance factor of a given Node
 */
static int TreeBalanceFactor(PCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return -1;
    }
    if ((NULL == Node->Left) && (NULL == Node->Right))
    {
        return 0;
    }
    else
    {
        if (NULL == Node->Left)
        {
            return (-1 - Node->Right->Height);
        }
        else
        {
            if (NULL == Node->Right)
            {
                return (Node->Left->Height + 1);
            }
            else
            {
                return (Node->Left->Height - Node->Right->Height);
            }
        }
    }
}

/**
 *Performs a double right rotation
 *
 *@param Node: a Node of the Tree
 *@return: -1 if an error occurs, otherwise 0
 */
static PCC_NODE_TREE TreeDoubleRightRotation(PCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return NULL;
    }
    PCC_NODE_TREE nodeRoot = Node; //k2
    PCC_NODE_TREE nodeLeft = Node->Left; //k1
    PCC_NODE_TREE nodeLeftRight = Node->Left->Right; //k3
    PCC_NODE_TREE nodeLeftRightLeft = Node->Left->Right->Left; //k3.left
    PCC_NODE_TREE nodeLeftRightRight = Node->Left->Right->Right; //k3.right

    PCC_NODE_TREE newRoot = nodeLeftRight;
    newRoot->Left = nodeLeft;
    newRoot->Right = nodeRoot;
    nodeLeft->Right = nodeLeftRightLeft;
    nodeRoot->Left = nodeLeftRightRight;
    TreeRecomputeHeight(nodeLeft);
    TreeRecomputeHeight(nodeRoot);
    TreeRecomputeHeight(newRoot);
    return newRoot;
}

static PCC_NODE_TREE TreeDoubleLeftRotation(PCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return NULL;
    }
    PCC_NODE_TREE nodeRoot = Node; //k2
    PCC_NODE_TREE nodeRight = Node->Right; //k1
    PCC_NODE_TREE nodeRightLeft = Node->Right->Left; //k3
    PCC_NODE_TREE nodeRightLeftLeft = Node->Right->Left->Left; //k3.left
    PCC_NODE_TREE nodeRightLeftRight = Node->Right->Left->Right; //k3.right

    PCC_NODE_TREE newRoot = nodeRightLeft;
    newRoot->Left = nodeRoot;
    newRoot->Right = nodeRight;
    nodeRight->Left = nodeRightLeftRight;
    nodeRoot->Right = nodeRightLeftLeft;
    TreeRecomputeHeight(nodeRight);
    TreeRecomputeHeight(nodeRoot);
    TreeRecomputeHeight(newRoot);
    return newRoot;
}

static PCC_NODE_TREE TreeSingleLeftRotation(PCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return NULL;
    }
    PCC_NODE_TREE nodeRoot = Node; //k1
    PCC_NODE_TREE nodeRight = Node->Right; //k2
    PCC_NODE_TREE nodeRightLeft = Node->Right->Left; //k3

    PCC_NODE_TREE newRoot = nodeRight;
    newRoot->Left = nodeRoot;
    nodeRoot->Right = nodeRightLeft;
    TreeRecomputeHeight(newRoot->Left);
    TreeRecomputeHeight(newRoot);
    return newRoot;
}

static PCC_NODE_TREE TreeSingleRightRotation(PCC_NODE_TREE Node)
{
    if (NULL == Node)
    {
        return NULL;
    }
    PCC_NODE_TREE nodeRoot = Node; //k1
    PCC_NODE_TREE nodeLeft = Node->Left; //k2
    PCC_NODE_TREE nodeLeftRight = Node->Left->Right; //k3

    PCC_NODE_TREE newRoot = nodeLeft;
    newRoot->Right = nodeRoot;
    nodeRoot->Left = nodeLeftRight;
    TreeRecomputeHeight(newRoot->Right);
    TreeRecomputeHeight(newRoot);
    return newRoot;
}

static int TreeMaxHeightTwoNodes(int HeightFirstNode, int HeightSecondNode)
{
    return HeightFirstNode > HeightSecondNode ? HeightFirstNode : HeightSecondNode;
}

static int TreeMinimumKey(PCC_NODE_TREE Node, int *Value)
{
    if (NULL == Node)
    {
        return -1;
    }
    PCC_NODE_TREE currentNode = Node;
    while (NULL != currentNode->Left)
    {
        currentNode = currentNode->Left;
    }
    *Value = currentNode->Key;
    currentNode = NULL;
    return 0;
}

