#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "bincode.h"

using namespace std;

class Node
{
public:
    unsigned char ch;
    unsigned int freq;
    BinCode code;
    Node *left;
    Node *right;

    Node();
    Node(char);
    Node(const Node&);
    ~Node();
    Node& operator=(const Node&);
    void updateCode(BinCode);
    friend ostream& operator<<(ostream&, const Node&);
};

#endif // NODE_H
