#include <iostream>
#include "../common/bincode.h"
#include "../common/tree.h"

using namespace std;

double seconds = 0;

void dynamicHuffman(char *inputName, char *outputName) {

    clock_t start, end;

    ifstream input(inputName, ios::binary);
    if (!input)
        return;
    ofstream output(outputName, ios::binary);
    if (!output)
        return;

    uint32_t bytesCount = 0;
    input.read((char*)(&bytesCount), sizeof(bytesCount));

    Tree tree;
    Node *node = &tree.root;

    Node *ascii[256] = {nullptr};

    unsigned char ch;
    int bits = 0;
    while (!input.eof()) {
        unsigned char byte = input.get();
        if (input.fail())
            break;

        for (int i = 7; i >= 0; i--) {
            bool bit = (byte >> i) & 1;
            if (bits == 0) {
                if (node->left == nullptr && node->right == nullptr) {
                    if (node == tree.esc) {
                        ch = (ch << 1) + bit;
                        bits = 1;
                    } else {
                        output << node->ch;
                        bytesCount--;
                        if (bytesCount == 0)
                            break;
                        start = clock();
                        tree.ReBuildTree(ascii[node->ch]);
                        end = clock();
                        seconds += double(end - start) / double(CLOCKS_PER_SEC);
                        node = &tree.root;
                    }
                }
                if (bit == 0)
                    node = node->left;
                else
                    node = node->right;
            } else {
                ch = (ch << 1) + bit;
                if (++bits == 8) {
                    output << ch;
                    bytesCount--;
                    start = clock();
                    ascii[ch] = tree.addNode(ch);
                    tree.ReBuildTree(ascii[ch]);
                    end = clock();
                    seconds += double(end - start) / double(CLOCKS_PER_SEC);
                    bits = 0;
                    node = &tree.root;
                }
            }
        }
        if (bytesCount == 0)
            break;
    }

    input.close();
    output.close();
}

int main()
{
    /*
    char fileName[256];

    cout << "file to decode: ";
    cin >> fileName;
    */

    dynamicHuffman("../Result", "../Reverse");

    cout << "\nThe time: " << seconds << " seconds\n";

    return 0;
}
