#include "linkednode.h"

LinkedNode::LinkedNode() : Node() {
    parent = nullptr;
    prev = next = nullptr;
}

LinkedNode::LinkedNode(char ch) : Node(ch) {
    parent = nullptr;
    prev = next = nullptr;
}

LinkedNode::LinkedNode(const LinkedNode& lnode) : Node(lnode) {
    parent = lnode.parent;
    prev = lnode.prev;
    next = lnode.next;
}

LinkedNode& LinkedNode::operator=(const LinkedNode& lnode) {
    if (this != &lnode) {
        Node::operator=(lnode);
        parent = lnode.parent;
        prev = lnode.prev;
        next = lnode.next;
    }
    return *this;
}

void LinkedNode::swap(LinkedNode *first, LinkedNode *second) {
    Node **u, **v;
    if (first->parent->left == first)
        u = &(first->parent->left);
    else
        u = &(first->parent->right);
    if (second->parent->left == second)
        v = &(second->parent->left);
    else
        v = &(second->parent->right);
    std::swap(*u, *v);
    std::swap(first->parent, second->parent);

    if (first->prev == second) {
        first->next->prev = second;
        second->next = first->next;
        second->prev->next = first;
        first->prev = second->prev;
        first->next = second;
        second->prev = first;
    } else {
        first->prev->next = second;
        first->next->prev = second;
        second->prev->next = first;
        second->next->prev = first;

        std::swap(first->prev, second->prev);
        std::swap(first->next, second->next);
    }
    std::swap(first->code, second->code);
    first->updateCode(first->code);
    second->updateCode(second->code);
}
