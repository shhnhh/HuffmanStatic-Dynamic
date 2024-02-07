#include "tree.h"

Tree::Tree() {
    esc = &root;
}

Node* Tree::addNode(char ch) {
    esc->right = new LinkedNode(ch);
    esc->next = (LinkedNode*)esc->right;
    esc->right->code = (esc->code << 1) | 1;
    ((LinkedNode*)(esc->right))->parent = esc;
    ((LinkedNode*)esc->right)->prev = esc;

    esc->left = new LinkedNode;
    ((LinkedNode*)esc->right)->next = (LinkedNode*)esc->left;
    esc->left->code = esc->code << 1;
    ((LinkedNode*)esc->left)->parent = esc;
    ((LinkedNode*)esc->left)->prev = (LinkedNode*)esc->right;

    esc = (LinkedNode*)esc->left;

    return esc->prev;
}

void Tree::ReBuildTree(Node *node) {
    LinkedNode *lnode = (LinkedNode*)node;
    while (true) {
        lnode->freq++;
        if (lnode == &root)
            break;
        else if (lnode->freq > lnode->prev->freq) {
            LinkedNode *p = lnode->prev;
            while (p->prev != nullptr && lnode->freq > p->prev->freq)
                p = p->prev;
            if (lnode->parent != p)
                LinkedNode::swap(lnode, p);
        }
        lnode = (LinkedNode*)lnode->parent;
    }
}
