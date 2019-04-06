#pragma once
#include "ccvector.h"

typedef struct _CC_NODE_HT {
    char *Key;
    int Value;
    struct _CC_NODE_HT *Next;
}CC_NODE_HT, *PCC_NODE_HT, **PPCC_NODE_HT;

typedef struct _CC_HASH_TABLE { 
    PCC_NODE_HT *Elements;
    int NumberOfElements;
} CC_HASH_TABLE, *PCC_HASH_TABLE, **PPCC_HASH_TABLE; 
  
int HtCreate(PPCC_HASH_TABLE HashTable); 
int HtDestroy(PPCC_HASH_TABLE HashTable); 
 
int HtSetKeyValue(PCC_HASH_TABLE HashTable, char* Key, int Value);
int HtGetKeyValue(PCC_HASH_TABLE HashTable, char* Key, int *Value);
int HtRemoveKey(PCC_HASH_TABLE HashTable, char* Key);
int HtHasKey(PCC_HASH_TABLE HashTable, char* Key);
int HtGetNthKey(PCC_HASH_TABLE HashTable, int Index, char** Key); 
int HtClear(PCC_HASH_TABLE HashTable); 
int HtGetKeyCount(PCC_HASH_TABLE HashTable);  
