#ifndef LINKEDNODE_H
#define LINKEDNODE_H

#include "node.h"

class LinkedNode : public Node
{

public:
    LinkedNode *parent;
    LinkedNode *prev, *next;

    LinkedNode();
    LinkedNode(char);
    LinkedNode(const LinkedNode&);
    LinkedNode& operator=(const LinkedNode&);

    static void swap(LinkedNode*, LinkedNode*);

};

#endif // LINKEDNODE_H
