#ifndef _INC_BPT_TREE_NODE_
#define _INC_BPT_TREE_NODE_

#include "../python/python.h"

typedef struct PbtNode
{
    PyObject*           locals;
    struct PbtNode**    children;
    uint8_t             children_count;

} PbtNode;

PbtNode * pbt_create_node();
void pbt_node_free(PbtNode* node);
void pbt_node_delete(PbtNode* node);

#endif