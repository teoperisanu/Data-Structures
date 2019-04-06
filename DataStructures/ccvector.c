#include "ccvector.h"
#include <stdlib.h>

#ifndef MAX_CAPACITY
#define MAX_CAPACITY 50
#endif

static int VecExtendCapacity(PCC_VECTOR Vector);
static int VecReduceCapacity(PCC_VECTOR Vector, int NewSize);

/**
 *Initializes a Vector structure
 *
 *@param Vector: a pointer to another pointer(the address of the pointer) to a structure of Vector type
 *@return: -1 if an error occurs, otherwise 0
 */
int VecCreate(PPCC_VECTOR Vector)
{
    //checks if the parameter is null
    if (NULL == Vector)
    {
        return -1;
    }
    //allocates memory for a new vector
    PCC_VECTOR tempVector = (PCC_VECTOR)malloc(sizeof(CC_VECTOR));
    if (NULL == tempVector)
    {
        return -1;
    }
    //allocates memory for the array
    int *tempArray = (int*)malloc(sizeof(int)*MAX_CAPACITY);
    if (NULL == tempArray)
    {
        free(tempVector);
        tempVector = NULL;
        return -1;
    }
    //associates the atributes with the structure
    *Vector = tempVector;
    (*Vector)->Elements = tempArray;
    (*Vector)->Capacity = MAX_CAPACITY;
    (*Vector)->Size = 0;
    tempVector = NULL;
    tempArray = NULL;
    return 0;
}

/**
 *Distroys a Vector and all its content
 *
 *@param Vector: a pointer to another pointer(the address of the pointer) to a structure of Vector type
 *@return: -1 if an error occurs, otherwise 0
 */
int VecDestroy(PPCC_VECTOR Vector)
{
    //checks if the pointer exists
    if (NULL == Vector)
    {
        return -1;
    }
    //checks if the Vector is already destroyed
    if (NULL == *Vector)
    {
        return 0;
    }
    //checks if the elements exists anymore
    if (NULL != (*Vector)->Elements)
    {
        //destroys elements
        free((*Vector)->Elements);
        (*Vector)->Elements = NULL;
    }
    //destroys the Vector
    (*Vector)->Capacity = 0;
    (*Vector)->Size = 0;
    free(*Vector);
    *Vector = NULL;
    return 0;
}

/**
 *Inserts a new value at the last position of the vector
 *
 *@param Vector: a pointer to a structure of Vector type
 *@param Value: the new value that will be addded
 *@return: -1 if an error occurs, otherwise 0
 */
int VecInsertTail(PCC_VECTOR Vector, int Value)
{
    //checks if the Vector exists
    if (NULL == Vector)
    {
        return -1;
    }
    //checks if in the Vector is enough space for adding a new element
    if (Vector->Size == Vector->Capacity)
    {
        //extending its capacity
        //in case of failure, returns an error code
        if (0 != VecExtendCapacity(Vector))
        {
            return -1;
        }
    }
    //inserting the element in the tail
    Vector->Elements[Vector->Size] = Value;
    Vector->Size++;
    return 0;
}

/**
 *Inserts a new value at the first position of the vector
 *
 *@param Vector: a pointer to a structure of Vector type
 *@param Value: the new value that will be addded
 *@return: -1 if an error occurs, otherwise 0
 */
int VecInsertHead(PCC_VECTOR Vector, int Value)
{
    //checks if the Vector exists
    if (NULL == Vector)
    {
        return -1;
    }
    //checks if in the Vector is enough space for adding a new element
    if (Vector->Size == Vector->Capacity)
    {
        //extending its capacity
        //in case of failure, returns an error code
        if (0 != VecExtendCapacity(Vector))
        {
            return -1;
        }
    }
    for (int index = Vector->Size - 1; index >= 0; index--)
    {
        Vector->Elements[index + 1] = Vector->Elements[index];
    }
    Vector->Elements[0] = Value;
    Vector->Size++;
    return 0;
}

/**
 *Inserts a new value after an element with a specific index
 *
 *@param Vector: a pointer to a structure of Vector type
 *@param Index: the index after that will be added the new value
 *@param Value: the new value that will be addded
 *@return: -1 if an error occurs, otherwise 0
 */
int VecInsertAfterIndex(PCC_VECTOR Vector, int Index, int Value)
{
    //checks if the Vector exists
    if (NULL == Vector)
    {
        return -1;
    }
    //checks if the Index is valid
    if ((Index < 0) || (Index > (Vector->Size - 1)))
    {
        return -1;
    }
    //checks if in the Vector is enough space for adding a new element
    if (Vector->Size == Vector->Capacity)
    {
        //extending its capacity
        //in case of failure, returns an error code
        if (0 != VecExtendCapacity(Vector))
        {
            return -1;
        }
    }
    //moving the elements between the Index and the tail with 1 position to the right
    for (int indexVec = Vector->Size - 1; indexVec > Index; indexVec--)
    {
        Vector->Elements[indexVec + 1] = Vector->Elements[indexVec];
    }
    //inserting the new element
    Vector->Elements[Index + 1] = Value;
    Vector->Size++;
    return 0;
}

/**
 *Removes a value from a specific index
 *
 *@param Vector: a pointer to a structure of Vector type
 *@param Index: the index of the element that will be deleted
 *@return: -1 if an error occurs, otherwise 0
 */
int VecRemoveByIndex(PCC_VECTOR Vector, int Index)
{
    //checks if the Vector exists
    if (NULL == Vector)
    {
        return -1;
    }
    //checks if the Index is valid
    if ((Index < 0) || (Index > (Vector->Size - 1)))
    {
        return -1;
    }
    //checks if in the Vector is too much unused space
    if ((Vector->Size <= (Vector->Capacity/4)) && (Vector->Capacity > MAX_CAPACITY))
    {
        //reducing its capacity
        int n = Vector->Capacity / 2;
        if (0 != VecReduceCapacity(Vector, n))
        {
            return -1;
        }
    }
    //moving the elements between the Index-1 and the tail with 1 position to the left
    for (int indexVec = Index; indexVec < Vector->Size - 1; indexVec++)
    {
        Vector->Elements[indexVec] = Vector->Elements[indexVec + 1];
    }
    //sets the new size
    Vector->Size--;

    return 0;
}

/**
 *Gets a value from the Vector at a specific position
 *
 *@param Vector: a pointer to a structure of Vector type
 *@param Index: the index of the element that will be deleted
 *@param Value: a place in memory, where the desired value will be placed
 *@return: -1 if an error occurs, otherwise 0
 */
int VecGetValueByIndex(PCC_VECTOR Vector, int Index, int *Value)
{
    //checks if the parameters are valid
    if (NULL == Vector || NULL == Value)
    {
        return -1;
    }
    //checks if the index is valid
    if ((Index < 0) || (Index > (Vector->Size - 1)))
    {
        return -1;
    }
    //geting the value
    *Value = Vector->Elements[Index];

    return 0;
}

/**
 *Gets the number of elements from the Vector
 *
 *@param Vector: a pointer to a structure of Vector type
 *@return: -1 if an error occurs, otherwise the number of elements
 */
int VecGetCount(PCC_VECTOR Vector)
{
    if (NULL == Vector)
    {
        return -1;
    }
    return Vector->Size;
}

/**
 *Removes all elements from Vector and sets the Vector to default size
 *
 *@param Vector: a pointer to a structure of Vector type
 *@return: -1 if an error occurs, otherwise 0
 */
int VecClear(PCC_VECTOR Vector)
{
    //checks if the Vector exists
    if (NULL == Vector)
    {
        return -1;
    }
    //sets its size to 0
    Vector->Size = 0;

    //resizing the Vector to default capacity
    if ((Vector->Size <= (Vector->Capacity / 4)) && (Vector->Capacity > MAX_CAPACITY))
    {
        if (0 != VecReduceCapacity(Vector, MAX_CAPACITY))
        {
            return -1;
        }
    }

    return 0;
}

static void VecQuickSort(PCC_VECTOR Vector, int StartingIndex, int EndingIndex);

/**
 *Sorts ascending a Vector
 *
 *@param Vector: a pointer to a structure of Vector type
 *@return: -1 if an error occurs, otherwise 0
 */
int VecSort(PCC_VECTOR Vector)
{
    if (NULL == Vector)
    {
        return -1;
    }
    VecQuickSort(Vector, 0, Vector->Size - 1);
    return 0;
}

/**
 *Doubles the capacity of a Vector
 *
 *@param Vector: a pointer to a structure of Vector type
 *@return: -1 if an error occurs, otherwise 0
 */
static int VecExtendCapacity(PCC_VECTOR Vector)
{
    //checks if the Vector exists
    if (NULL == Vector)
    {
        return -1;
    }
    //reallocate the Vector
    int *newArray = (int*)realloc(Vector->Elements, (sizeof(Vector->Elements))*(2* Vector->Capacity));
   
    if (NULL == newArray)
    {
        //realloc failed, but the initial array is still valid
        return -1;
    }

    //reallocation succeeded, the old memory-zone was freed automatically by realloc
    //update the new data
    Vector->Elements = newArray;
    Vector->Capacity *= 2;
    newArray = NULL;
    return 0;
}

/**
 *Reduces the Vector capacity with a given size
 *
 *@param Vector: a pointer to a structure of Vector type
 *@return: -1 if an error occurs, otherwise 0
 */
static int VecReduceCapacity(PCC_VECTOR Vector, int NewSize)
{
    if (NULL == Vector || NewSize < 0)
    {
        return -1;
    }

    int *newArray = (int*)realloc(Vector->Elements, NewSize * sizeof(Vector->Elements));
    if (NULL == newArray)
    {
        //realloc failed, but the initial array is still valid
        return -1;
    }

    //reallocation succeeded, the old memory-zone was freed automatically by realloc
    //update the new data
    Vector->Elements = newArray;
    Vector->Capacity = NewSize;
    newArray = NULL;
    return 0;
}

/**
 *Swap the values between two numbers
 *
 *@param FirstElement: integer
 *@param SecondElement: integer
 */
static void VecSwapTwoElements (int *FirstElement, int *SecondElement)
{
    (*FirstElement) ^= (*SecondElement) ^= (*FirstElement) ^= (*SecondElement);
}

static int VecPartition(PCC_VECTOR Vector, int StartingIndex, int EndingIndex)
{
    int pivotElement = Vector->Elements[EndingIndex];
    int index1 = StartingIndex - 1;
    for (int index2 = StartingIndex; index2 < EndingIndex; index2++)
    {
        if (Vector->Elements[index2] <= pivotElement)
        {
            index1++;
            VecSwapTwoElements(&Vector->Elements[index1], &Vector->Elements[index2]);
        }
    }
    VecSwapTwoElements(&Vector->Elements[index1 + 1], &Vector->Elements[EndingIndex]);
    return (index1 + 1);
}

static void VecQuickSort(PCC_VECTOR Vector, int StartingIndex, int EndingIndex)
{
    if (StartingIndex < EndingIndex)
    {
        int index = VecPartition(Vector, StartingIndex, EndingIndex);
        VecQuickSort(Vector, StartingIndex, index - 1);
        VecQuickSort(Vector, index + 1, EndingIndex);
    }
}