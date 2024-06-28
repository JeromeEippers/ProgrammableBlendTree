#include "node.h"
#include <malloc.h>


PbtNode * pbt_create_node()
{
    PbtNode * node = calloc(1, sizeof(PbtNode));
    return node;
}

void pbt_node_free(PbtNode* node)
{
    if (node->children != NULL) free(node->children);
    if (node->locals != NULL) Py_DecRef(node->locals);
    node->locals = NULL;
    node->children = NULL;
    node->children_count = 0;
}

void pbt_node_delete(PbtNode* node)
{
    pbt_node_free(node);
    free(node);
}