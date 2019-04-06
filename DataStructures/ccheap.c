#include "ccheap.h"
#include <stdlib.h>

#ifndef DEFAULT_CAPACITY
#define DEFAULT_CAPACITY 50
#endif

static int HpGetParent(int ChildIndex);
static int HpGetLeft(int ParentIndex);
static int HpGetRight(int ParentIndex);
static void HpSwapTwoElements(int *FirstElement, int *SecondElement);
static int HpMaxHeapify(PCC_HEAP MaxHeap, int Index);
static int HpMinHeapify(PCC_HEAP MinHeap, int Index);
static int HpMaxHeapIncreaseKey(PCC_HEAP MaxHeap, int StartingIndex);
static int HpMinHeapIncreaseKey(PCC_HEAP MinHeap, int StartingIndex);

int HpCreateMaxHeap(PPCC_HEAP MaxHeap, PCC_VECTOR InitialElements)
{
    if (NULL == MaxHeap)
    {
        return -1;
    }
    PCC_HEAP tempHeap = (PCC_HEAP)malloc(sizeof(CC_HEAP));
    if (NULL == tempHeap)
    {
        return -1;
    }
    (*MaxHeap) = tempHeap;
    (*MaxHeap)->Type = 1;
    
    if (NULL != InitialElements)
    {
        (*MaxHeap)->Data = InitialElements;
        for (int index = InitialElements->Size / 2; index >= 0; index--)
        {
            HpMaxHeapify((*MaxHeap), index);
        }

    }
    else
    {
        PCC_VECTOR Data = NULL;
        if (-1 == VecCreate(&Data))
        {
            return -1;
        }
        (*MaxHeap)->Data = Data;
    }
    return 0;
}

int HpCreateMinHeap(PPCC_HEAP MinHeap, PCC_VECTOR InitialElements)
{
    if (NULL == MinHeap)
    {
        return -1;
    }
    PCC_HEAP tempHeap = (PCC_HEAP)malloc(sizeof(CC_HEAP));
    if (NULL == tempHeap)
    {
        return -1;
    }
    (*MinHeap) = tempHeap;
    (*MinHeap)->Type = 0;

    if (NULL != InitialElements)
    {
        (*MinHeap)->Data = InitialElements;
        for (int index = InitialElements->Size / 2; index >= 0; index--)
        {
            HpMinHeapify((*MinHeap), index);
        }
    }
    else
    {
        PCC_VECTOR Data = NULL;
        if (-1 == VecCreate(&Data))
        {
            return -1;
        }
        (*MinHeap)->Data = Data;
    }
    return 0;
}

int HpDestroy(PPCC_HEAP Heap)
{
    if (NULL == Heap)
    {
        return -1;
    }
    
    if (NULL == *Heap)
    {
        return 0;
    }
    
    if (NULL != (*Heap)->Data)
    {
       
        if (-1 == VecDestroy(&((*Heap)->Data)))
        {
            return -1;
        }
        (*Heap)->Data = NULL;
    }
   
    (*Heap)->Type = 0;
    free(*Heap);
    *Heap = NULL;
    return 0;
}

int HpInsert(PCC_HEAP Heap, int Value)
{
    if (NULL == Heap)
    {
        return -1;
    }
    if (-1 == VecInsertTail(Heap->Data, Value))
    {
        return -1;
    }
    if (1 == Heap->Type)
    {
        HpMaxHeapIncreaseKey(Heap, Heap->Data->Size - 1);
    }
    else
    {
        HpMinHeapIncreaseKey(Heap, Heap->Data->Size - 1);
    }
    return 0;
}

int HpRemove(PCC_HEAP Heap, int Value)
{
    if (NULL == Heap)
    {
        return -1;
    }
    if (0 == Heap->Data->Size)
    {
        return -1;
    }
    int found = 0;
    do{
        for (int index = 0; index < Heap->Data->Size; index++)
        {
            found = 0;
            if (Heap->Data->Elements[index] == Value)
            {
                found = 1;
                Heap->Data->Elements[index] = Heap->Data->Elements[Heap->Data->Size - 1];
                int parentIndex = HpGetParent(index);
                //min heap
                if (0 == Heap->Type)
                {
                    if ((0 == index) || (Heap->Data->Elements[parentIndex] < Heap->Data->Elements[index]))
                    {
                        HpMinHeapify(Heap, index);
                    }
                    else
                    {
                        HpMinHeapIncreaseKey(Heap, index);
                    }
                }
                else
                {
                    if ((0 == index) || (Heap->Data->Elements[parentIndex] > Heap->Data->Elements[index]))
                    {
                        HpMaxHeapify(Heap, index);
                    }
                    else
                    {
                        HpMaxHeapIncreaseKey(Heap, index);
                    }
                }
                break;
            }
        }
    } while (1 == found);
    return 0;
}

int HpGetExtreme(PCC_HEAP Heap, int* ExtremeValue)
{
    if (NULL == Heap || NULL == ExtremeValue)
    {
        return -1;
    }
    if (Heap->Data->Size < 1)
    {
        return -1;
    }
    return VecGetValueByIndex(Heap->Data, 0, ExtremeValue);
}

int HpPopExtreme(PCC_HEAP Heap, int* ExtremeValue)
{
    if (NULL == Heap || NULL == ExtremeValue)
    {
        return -1;
    }
    if (Heap->Data->Size < 1)
    {
        return -1;
    }
    if (-1 == VecGetValueByIndex(Heap->Data, 0, ExtremeValue))
    {
        return -1;
    }
    HpSwapTwoElements(&Heap->Data->Elements[0], &Heap->Data->Elements[Heap->Data->Size - 1]);
    VecRemoveByIndex(Heap->Data, Heap->Data->Size - 1);
    if (1 == Heap->Type)
    {
        HpMaxHeapify(Heap, 0);
    }
    else
    {
        HpMinHeapify(Heap, 0);
    }
    return 0;
}

int HpGetElementCount(PCC_HEAP Heap)
{
    if (NULL == Heap)
    {
        return -1;
    }

    return VecGetCount(Heap->Data);
}

int HpSortToVector(PCC_HEAP Heap, PCC_VECTOR SortedVector)
{
    if (NULL == Heap || NULL == SortedVector)
    {
        return -1;
    }
    int sizeOfHeap = Heap->Data->Size;
    
    if (1 == Heap->Type)
    {
        int max = 0;
        while (Heap->Data->Size > 1)
        {
            HpPopExtreme(Heap, &max);
            VecInsertHead(SortedVector, max);
        }
        VecInsertHead(SortedVector, Heap->Data->Elements[0]);
        Heap->Data->Size = sizeOfHeap;
        for (int index = sizeOfHeap / 2; index >= 0; index--)
        {
            HpMaxHeapify(Heap, index);
        }
    }
    else
    {
        int min = 0;
        while (Heap->Data->Size > 1)
        {
            HpPopExtreme(Heap, &min);
            VecInsertTail(SortedVector, min);
        }
        VecInsertTail(SortedVector, Heap->Data->Elements[0]);
        Heap->Data->Size = sizeOfHeap;
        for (int index = sizeOfHeap / 2; index >= 0; index--)
        {
            HpMinHeapify(Heap, index);
        }
    }
    
    return 0;
}

static int HpGetParent(int ChildIndex)
{
    if (ChildIndex < 0)
    {
        return -1;
    }
    return (ChildIndex - 1) / 2;
}

static int HpGetLeft(int ParentIndex)
{
    if (ParentIndex < 0)
    {
        return -1;
    }
    return 2 * ParentIndex + 1;
}

static int HpGetRight(int ParentIndex)
{
    if (ParentIndex < 0)
    {
        return -1;
    }
    return 2 * ParentIndex + 2;
}

static void HpSwapTwoElements(int *FirstElement, int *SecondElement)
{
    (*FirstElement) ^= (*SecondElement) ^= (*FirstElement) ^= (*SecondElement);
}


static int HpMaxHeapify(PCC_HEAP MaxHeap, int Index)
{
    if (NULL == MaxHeap)
    {
        return -1;
    }
    if ((Index < 0) || (Index >= MaxHeap->Data->Size))
    {
        return -1;
    }
    int leftChild = HpGetLeft(Index);
    int rightChild = HpGetRight(Index);
    int largestElemIndex = Index;
    if ((leftChild < MaxHeap->Data->Size) && (MaxHeap->Data->Elements[leftChild] > MaxHeap->Data->Elements[Index]))
    {
        largestElemIndex = leftChild;
    }
    if ((rightChild < MaxHeap->Data->Size) && (MaxHeap->Data->Elements[rightChild] > MaxHeap->Data->Elements[largestElemIndex]))
    {
        largestElemIndex = rightChild;
    }
    if (largestElemIndex != Index)
    {
        HpSwapTwoElements(&MaxHeap->Data->Elements[Index], &MaxHeap->Data->Elements[largestElemIndex]);
        HpMaxHeapify(MaxHeap, largestElemIndex);
    }
    return 0;
}

static int HpMaxHeapifyIterative(PCC_HEAP MaxHeap, int Index)
{
    if (NULL == MaxHeap)
    {
        return -1;
    }
    if ((Index < 0) || (Index >= MaxHeap->Data->Size))
    {
        return -1;
    }
    int maxChild;
    while (Index < MaxHeap->Data->Size)
    {
        maxChild = -1;
        if (HpGetLeft(Index) < MaxHeap->Data->Size)
        {
            maxChild = HpGetLeft(Index);
        }
        if ((HpGetRight(Index) < MaxHeap->Data->Size) && (MaxHeap->Data->Elements[HpGetRight(Index)] > MaxHeap->Data->Elements[HpGetLeft(Index)]))
        {
            maxChild = HpGetRight(Index);
        }
        if ((maxChild != -1) && (MaxHeap->Data->Elements[maxChild] > MaxHeap->Data->Elements[Index]))
        {
            MaxHeap->Data->Elements[Index] = MaxHeap->Data->Elements[maxChild];
            Index = maxChild;
        }
        else
        {
            Index = MaxHeap->Data->Size;
        }
    }
    return 0;
}

static int HpMinHeapify(PCC_HEAP MinHeap, int Index)
{
    if (NULL == MinHeap)
    {
        return -1;
    }
    if ((Index < 0) || (Index >= MinHeap->Data->Size))
    {
        return -1;
    }
    int leftChild = HpGetLeft(Index);
    int rightChild = HpGetRight(Index);
    int smallestElemIndex = Index;
    if ((leftChild < MinHeap->Data->Size) && (MinHeap->Data->Elements[leftChild] < MinHeap->Data->Elements[Index]))
    {
        smallestElemIndex = leftChild;
    }
    if ((rightChild < MinHeap->Data->Size) && (MinHeap->Data->Elements[rightChild] < MinHeap->Data->Elements[smallestElemIndex]))
    {
        smallestElemIndex = rightChild;
    }
    if (smallestElemIndex != Index)
    {
        HpSwapTwoElements(&MinHeap->Data->Elements[Index], &MinHeap->Data->Elements[smallestElemIndex]);
        HpMinHeapify(MinHeap, smallestElemIndex);
    }
    return 0;
}

static int HpMaxHeapIncreaseKey(PCC_HEAP MaxHeap, int StartingIndex)
{
    if (NULL == MaxHeap)
    {
        return -1;
    }
    if ((StartingIndex < 0) || (StartingIndex >= MaxHeap->Data->Size))
    {
        return -1;
    }
    while ((0 < StartingIndex) && (MaxHeap->Data->Elements[HpGetParent(StartingIndex)] < MaxHeap->Data->Elements[StartingIndex]))
    {
        HpSwapTwoElements(&MaxHeap->Data->Elements[StartingIndex], &MaxHeap->Data->Elements[HpGetParent(StartingIndex)]);
        StartingIndex = HpGetParent(StartingIndex);
    }
    return 0;
}
static int HpMinHeapIncreaseKey(PCC_HEAP MinHeap, int StartingIndex)
{
    if (NULL == MinHeap)
    {
        return -1;
    }
    if ((StartingIndex < 0) || (StartingIndex >= MinHeap->Data->Size))
    {
        return -1;
    }
    while ((0 < StartingIndex) && (MinHeap->Data->Elements[HpGetParent(StartingIndex)] > MinHeap->Data->Elements[StartingIndex]))
    {
        HpSwapTwoElements(&MinHeap->Data->Elements[StartingIndex], &MinHeap->Data->Elements[HpGetParent(StartingIndex)]);
        StartingIndex = HpGetParent(StartingIndex);
    }
    return 0;
}