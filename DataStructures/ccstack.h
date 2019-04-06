#pragma once

typedef struct _CC_NODE_STACK {
    int Key;
    struct _CC_NODE_STACK *Next;
} CC_NODE_STACK, *PCC_NODE_STACK;

typedef struct _CC_STACK{ 
    PCC_NODE_STACK Top;
} CC_STACK, *PCC_STACK, **PPCC_STACK; 
  
int StCreate(PPCC_STACK Stack);
int StDestroy(PPCC_STACK Stack);
 
int StPush(PCC_STACK Stack, int Value);
int StPop(PCC_STACK Stack, int *Value);
int StPeek(PCC_STACK Stack, int *Value); // gets top of stack without popping the value
int StIsEmpty(PCC_STACK Stack); // returns 1 if stack is empty and 0 otherwise
int StGetCount(PCC_STACK Stack); // returns the number of elements in the stack
int StClear(PCC_STACK Stack); // removes all elements from the stack

// StPushStack removes all the elements from the StackToPush and appends 
// them to the first stack 
// ex: Stack1: 1, 2, 3
//     Stack2: 1, 4, 5
// After push: Stack1: 1, 2, 3, 1, 4, 5
//             Stack2: empty
int StPushStack(PCC_STACK Stack,PCC_STACK StackToPush); 
