#pragma once

typedef struct _CC_VECTOR { 
    // Members:
    int *Elements;//effective array that contains the elements
    int Capacity;//the maximum size of the array
    int Size;//the number of the elements that are in the array

} CC_VECTOR, *PCC_VECTOR, **PPCC_VECTOR; 
  
int VecCreate(PPCC_VECTOR Vector);
int VecDestroy(PPCC_VECTOR Vector);
  
int VecInsertTail(PCC_VECTOR Vector, int Value);
int VecInsertHead(PCC_VECTOR Vector, int Value);
int VecInsertAfterIndex(PCC_VECTOR Vector, int Index, int Value);
int VecRemoveByIndex(PCC_VECTOR Vector, int Index);
int VecGetValueByIndex(PCC_VECTOR Vector, int Index, int *Value);
int VecGetCount(PCC_VECTOR Vector);
int VecClear(PCC_VECTOR Vector);
int VecSort(PCC_VECTOR Vector);
