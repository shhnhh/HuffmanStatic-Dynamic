#include "node.h"

Node::Node() {
    ch = 0;
    freq = 0;
    left = right = nullptr;
}

Node::Node(char ch) {
    this->ch = ch;
    freq = 0;
    left = right = nullptr;
}

Node::Node(const Node& node) {
    ch = node.ch;
    freq = node.freq;
    code = node.code;
    if (node.left != nullptr) {
        left = new Node(*node.left);
    } else
        left = nullptr;
    if (node.right != nullptr) {
        right = new Node(*node.right);
    } else
        right = nullptr;
}

Node::~Node() {
    delete left;
    delete right;
}

Node& Node::operator=(const Node& node) {
    if (&node != this) {
        delete left;
        delete right;
        ch = node.ch;
        freq = node.freq;
        code = node.code;
        if (node.left != nullptr)
            left = new Node(*node.left);
        else
            left = nullptr;
        if (node.right != nullptr)
            right = new Node(*node.right);
        else
            right = nullptr;
    }
    return *this;
}

void Node::updateCode(BinCode code) {
    this->code = code;
    if (left != nullptr)
        left->updateCode(code << 1);
    if (right != nullptr)
        right->updateCode((code << 1) | 1);
}

ostream& operator<<(ostream& os, const Node& node) {
    os << &node << '|' << node.left << '|' << node.right;
    return os;
}
