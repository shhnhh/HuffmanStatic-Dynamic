#include <iostream>
#include "../common/bincode.h"
#include "../common/tree.h"
#include <ctime>

using namespace std;

double seconds = 0;

fstream::pos_type fileSize(char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void dynamicHuffman(char *inputName, char *outputName) {

    clock_t start, end;

    uint32_t bytesCount = fileSize(inputName);

    ifstream input(inputName, ios::binary);
    if (!input)
        return;
    ofstream output(outputName, ios::binary);
    if (!output)
        return;

    output.write((char*)(&bytesCount), sizeof(bytesCount));

    Node* ascii[256] = {nullptr};

    Tree tree;

    BinCode bytes;
    while (!input.eof()) {
        unsigned char ch = input.get();
        if (input.fail())
            break;

        start = clock();

        BinCode code;
        if (ascii[ch] == nullptr) {
            code = (tree.esc->code << 8) | ch;
            ascii[ch] = tree.addNode(ch);
        } else
            code = ascii[ch]->code;

        tree.ReBuildTree(ascii[ch]);

        end = clock();
        seconds += double(end - start) / double(CLOCKS_PER_SEC);

        if (code.count > 64 - bytes.count) {
            int n = 64 - bytes.count;
            bytes = (bytes << n) | (code >> (code.count - n));
            for (int i = 7; i >= 0; i--)
                output.write((char*)&bytes.code + i, 1);
            bytes = (code << (64 - code.count + n)) >> (64 - code.count + n);
        } else
            bytes = (bytes << code.count) | code;
    }

    if (bytes.count != 0) {
        bytes <<= (8 - bytes.count % 8) % 8;
        for (int i = bytes.count / 8 - 1; i >= 0; i--)
            output.write((char*)&bytes.code + i, 1);
    }

    input.close();
    output.close();
}

int main()
{
    char fileName[256];

    cout << "file to encode: ";
    cin >> fileName;

    dynamicHuffman(fileName, "../Result");

    cout << "\nThe time: " << seconds << " seconds\n";

    return 0;
}
