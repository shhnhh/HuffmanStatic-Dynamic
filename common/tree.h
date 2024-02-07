#ifndef TREE_H
#define TREE_H

#include "linkednode.h"

class Tree
{

public:
    LinkedNode root;
    LinkedNode *esc;

    Tree();

    Node* addNode(char);
    void ReBuildTree(Node*);
};

#endif // TREE_H
