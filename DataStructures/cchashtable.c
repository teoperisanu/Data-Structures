#include "cchashtable.h"
#include <stdlib.h>

#ifndef SIZE
#define SIZE 20
#endif

static int HtHashFunction(PCC_HASH_TABLE HashTable, char *Key);
static int HtCompareKeys(char *FirstKey, char *SecondKey);

/*Creates a new Node for the HashTable*/
int static HtCreateNode(PPCC_NODE_HT Node)
{
    
    if (NULL == Node)
    {
        return -1;
    }
    PCC_NODE_HT tempNode = (PCC_NODE_HT)malloc(sizeof(CC_NODE_HT));
    if (NULL == tempNode)
    {
        return -1;
    }
    char *tempKey = (char*)malloc(50);
    if (NULL == tempKey)
    {
        free(tempNode);
        tempNode = NULL;
        return -1;
    }
    (*Node) = tempNode;
    (*Node)->Key = tempKey;
    (*Node)->Next = NULL;
    tempNode = NULL;
    return 0;
}

/*Destroys a given Node*/
int static HtDestroyNode(PPCC_NODE_HT Node)
{
    if (NULL == Node)
    {
        return -1;
    }
    if (NULL == (*Node))
    {
        return 0;
    }
    (*Node)->Key = NULL;
    (*Node)->Value = 0;
    (*Node)->Next = NULL;
    free((*Node));
    (*Node) = NULL;
    return 0;
}

/*Creates a new Hash Table*/
int HtCreate(PPCC_HASH_TABLE HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    PCC_HASH_TABLE tempHT = (PCC_HASH_TABLE)malloc(sizeof(CC_HASH_TABLE));
    if (NULL == tempHT)
    {
        return -1;
    }
    PCC_NODE_HT *tempArray = (PPCC_NODE_HT)calloc(SIZE, sizeof(PCC_NODE_HT));
    if (NULL == tempArray)
    {
        free(tempHT);
        tempHT = NULL;
        return -1;
    }
    (*HashTable) = tempHT;
    (*HashTable)->Elements = tempArray;
    (*HashTable)->NumberOfElements = 0;
    tempHT = NULL;
    tempArray = NULL;
    return 0;
}

/*Destroys A Hash Table*/
int HtDestroy(PPCC_HASH_TABLE HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == (*HashTable))
    {
        return 0;
    }
    if (0 != HtClear((*HashTable)))
    {
        return -1;
    }
    free((*HashTable)->Elements);
    (*HashTable)->NumberOfElements = 0;
    free((*HashTable));
    (*HashTable) = NULL;
    return 0;
}

/*Set or creates a node with a given Key-Value*/
int HtSetKeyValue(PCC_HASH_TABLE HashTable, char* Key, int Value)
{
    if (NULL == HashTable || NULL == Key)
    {
        return -1;
    }
    int index = HtHashFunction(HashTable, Key);
    PCC_NODE_HT currentNode = HashTable->Elements[index];
    while (NULL != currentNode)
    {
        if (0 == HtCompareKeys(currentNode->Key, Key))
        {
            currentNode->Value = Value;
            return 0;
        }
        currentNode = currentNode->Next;
    }

    PCC_NODE_HT newNode = NULL;
    if (0 != HtCreateNode(&newNode))
    {
        return -1;
    }
    newNode->Key = Key;
    newNode->Value = Value;
    if (NULL == currentNode)
    {
        HashTable->Elements[index] = newNode;
    }
    else
    {
        currentNode->Next = newNode;
    }
    HashTable->NumberOfElements++;
    newNode = NULL;

    return 0;
}

/*Gets the value of a Key*/
int HtGetKeyValue(PCC_HASH_TABLE HashTable, char* Key, int *Value)
{
    if (NULL == HashTable || NULL == Key)
    {
        return -1;
    }
    int index = HtHashFunction(HashTable, Key);
    PCC_NODE_HT currentNode = HashTable->Elements[index];
    while (NULL != currentNode)
    {
        if (0 == HtCompareKeys(currentNode->Key, Key))
        {
            *Value = currentNode->Value;
            break;
        }
        currentNode = currentNode->Next;
    }
    return 0;
}


int HtRemoveKey(PCC_HASH_TABLE HashTable, char* Key)
{
    if (NULL == HashTable || NULL == Key)
    {
        return -1;
    }
    int index = HtHashFunction(HashTable, Key);
    PCC_NODE_HT currentNode = HashTable->Elements[index];
    PCC_NODE_HT prevNode = NULL;

    while ((NULL != currentNode) && (0 != HtCompareKeys(currentNode->Key, Key)))
    {
        prevNode = currentNode;
        currentNode = currentNode->Next;
    }
    if (NULL == prevNode)
    {
        if (NULL != currentNode)
        {
            HashTable->Elements[index] = currentNode->Next;
            currentNode->Next = NULL;
            HtDestroyNode(&currentNode);
        }
    }
    else
    {
        if (NULL != currentNode)
        {
            prevNode->Next = currentNode->Next;
            currentNode->Next = NULL;
            HtDestroyNode(&currentNode);
        }
    }
    HashTable->NumberOfElements--;
    return 0;
}

int HtHasKey(PCC_HASH_TABLE HashTable, char* Key)
{
    if (NULL == HashTable || NULL == Key)
    {
        return -1;
    }
    int index = HtHashFunction(HashTable, Key);
    PCC_NODE_HT currentNode = HashTable->Elements[index];
    while (NULL != currentNode)
    {
        if (0 == HtCompareKeys(currentNode->Key, Key))
        {
            return 1;
        }
        currentNode = currentNode->Next;
    }
    return 0;

}

int HtGetNthKey(PCC_HASH_TABLE HashTable, int Index, char** Key)
{
    if (NULL == HashTable || NULL == Key)
    {
        return -1;
    }
    if (Index < 0 || Index >= HashTable->NumberOfElements)
    {
        return -1;
    }
    int count = 0;
    for (int index = 0; index < SIZE; index++)
    {
        PCC_NODE_HT currentNode = HashTable->Elements[index];
        while (NULL != currentNode)
        {
            if (count == Index)
            {
                *Key = currentNode->Key;
                return 0;
            }
            currentNode = currentNode->Next;
            count++;
        }
    }
    return 0;
}

int HtClear(PCC_HASH_TABLE HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    for (int index = 0; index < SIZE; index++)
    {
        PCC_NODE_HT currentNode = HashTable->Elements[index];
        while (NULL != currentNode)
        {
            PCC_NODE_HT tempNode = currentNode;
            currentNode = currentNode->Next;
            HashTable->Elements[index] = currentNode;
            HtDestroyNode(&tempNode);
        }
    }
    return 0;
}

int HtGetKeyCount(PCC_HASH_TABLE HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    return HashTable->NumberOfElements;
}

static int HtHashFunction(PCC_HASH_TABLE HashTable, char *Key)
{
    if (NULL == HashTable || NULL == Key)
    {
        return -1;
    }
    int sum = 0;
    while (*Key)
    {
        sum += *Key;
        Key++;
    }
    return (sum % SIZE);
}

static int HtCompareKeys(char *FirstKey, char *SecondKey)
{
    if (*FirstKey == '\0' || *FirstKey != *SecondKey)
    {
        return *FirstKey - *SecondKey;
    }
    else
    {
        return(HtCompareKeys(FirstKey + 1, SecondKey + 1));
    }
}
