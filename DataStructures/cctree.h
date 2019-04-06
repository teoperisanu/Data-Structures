#pragma once

typedef struct _CC_NODE_TREE {
    int Key;
    struct _CC_NODE_TREE *Left;
    struct _CC_NODE_TREE *Right;
    int Height;
    int Count;
}CC_NODE_TREE, *PCC_NODE_TREE, **PPCC_NODE_TREE;

typedef struct _CC_TREE {
    // Members
    PCC_NODE_TREE Root;
    int NumberOfNodes;
} CC_TREE, *PCC_TREE, **PPCC_TREE;

int TreeCreate(PPCC_TREE Tree);
int TreeDestroy(PPCC_TREE Tree);

// Duplicates are allowed
int TreeInsert(PCC_TREE Tree, int Value);

// Removes all elements equal to Value
int TreeRemove(PCC_TREE Tree, int Value);

/*
 *  Return:
 *      1   - Tree contains value
 *      0   - Tree does not contain Value
 */
int TreeContains(PCC_TREE Tree, int Value);

int TreeGetCount(PCC_TREE Tree);
int TreeGetHeight(PCC_TREE Tree);
int TreeClear(PCC_TREE Tree);

int TreeGetNthPreorder(PCC_TREE Tree, int Index, int *Value);
int TreeGetNthInorder(PCC_TREE Tree, int Index, int *Value);
int TreeGetNthPostorder(PCC_TREE Tree, int Index, int *Value);
