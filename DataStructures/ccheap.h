#pragma once

#include "ccvector.h"

typedef struct _CC_HEAP{ 
    //int *Elements;//effective array that contains the elements
    //int Capacity;//the maximum size of the array
    //int Size;//the number of the elements that are in the array
    PCC_VECTOR Data;
    unsigned int Type : 1; // 0 - MinHeap, 1 - MaxHeap
} CC_HEAP, *PCC_HEAP, **PPCC_HEAP; 
  
// HpCreateMaxHeap and HpCreateMinHeap should create a max heap or a min heap, 
// respectively. InitialElements is an optional parameter and, if it is not null, the constructed
// heap should initially contain all the elements in the provided vector.
int HpCreateMaxHeap(PPCC_HEAP MaxHeap, PCC_VECTOR InitialElements); 
int HpCreateMinHeap(PPCC_HEAP MinHeap, PCC_VECTOR InitialElements); 
int HpDestroy(PPCC_HEAP Heap); 

int HpInsert(PCC_HEAP Heap, int Value); 

// HpRemove should remove all elements with the value Value in the heap
int HpRemove(PCC_HEAP Heap, int Value); 

// HpGetExtreme should return the maximum/minumum value in the heap, depending on the 
// type of heap constructed
int HpGetExtreme(PCC_HEAP Heap, int* ExtremeValue);

// HpPopExtreme should return the maximum/minimum value in the heap, and remove all 
// instances of said value from the heap
int HpPopExtreme(PCC_HEAP Heap, int* ExtremeValue);

int HpGetElementCount(PCC_HEAP Heap);

// HpSortToVector should construct and return (in the SortedVector parameter) a vector 
// sorted in increasing order containing all the elements present in the heap
int HpSortToVector(PCC_HEAP Heap, PCC_VECTOR SortedVector);
