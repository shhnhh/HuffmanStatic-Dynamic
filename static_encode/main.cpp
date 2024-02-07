#include "../common/bincode.h"
#include "../common/node.h"
#include <cmath>
#include <ctime>

using namespace std;

void fillCodes(Node *node, BinCode bin, BinCode *codes) {
    if (node == nullptr)
        return;
    node->code = bin;
    if (node->left == nullptr && node->right == nullptr)
        codes[node->ch] = bin;
    fillCodes(node->left, bin << 1, codes);
    fillCodes(node->right, (bin << 1) | 1, codes);
}

void LSD_sort(Node *A, int n, int m, int k) {
    unsigned long long r = 1;
    int C[k];
    Node B[n];
    for (int i = 0; i < m; i++) {
        int d;
        for (int j = 0; j < k; j++)
            C[j] = 0;
        for (int j = 0; j < n; j++) {
            d = A[j].freq / r % k;
            C[d]++;
        }
        int tmp, count = 0;
        for (int j = 0; j < k; j++) {
            tmp = C[j];
            C[j] = count;
            count += tmp;
        }
        for (int j = 0; j < n; j++) {
            d = A[j].freq / r % k;
            B[C[d]] = A[j];
            C[d]++;
        }
        for (int j = 0; j < n; j++)
            A[j] = B[j];
        r *= k;
    }
}

void quicksort(Node *A, int l, int r) {
    int i = l, j = r;
    int m = (l + r) / 2;
    do {
         while (A[i].freq < A[m].freq)
             i++;
         while (A[j].freq > A[m].freq)
             j--;
         if (i <= j) {
             swap(A[i], A[j]);
             i++;
             j--;
         }
    } while (i < j);
    if (j > l)
        quicksort(A, l, j);
    if (i < r)
        quicksort(A, i, r);
}

Node buildTree(Node *arr1, int count) {

    Node arr2[count + 1];
    for (int i = 0; i < count + 1; i++)
        arr2[i].freq = INFINITY;

    int l1 = 0, l2 = 0, r2 = 0;

    for (int i = 0; i < count - 1; i++) {
        Node new_node, mn1, mn2;
        if (arr1[l1].freq <= arr2[l2].freq)
            mn1 = arr1[l1++];
        else
            mn1 = arr2[l2++];

        if (arr1[l1].freq <= arr2[l2].freq)
            mn2 = arr1[l1++];
        else
            mn2 = arr2[l2++];

        new_node.left = new Node(mn1);
        new_node.freq = mn1.freq;
        if (mn2.freq != INFINITY) {
            new_node.right = new Node(mn2);
            new_node.freq += mn2.freq;
        }
        arr2[r2++] = new_node;
    }

    return arr2[count - 2];
}

void readFile(char *fileName, Node *ascii, short& count) {
    ifstream file(fileName, ios::binary);
    if (!file)
        return;

    count = 0;

    while (!file.eof()) {
        unsigned char ch = file.get();
        if (file.fail())
            break;
        ascii[ch].freq++;
        if (ascii[ch].freq == 1)
            count++;
    }

    file.close();
}

void staticHuffman(Node *ascii, short count, BinCode *codes) {

    Node arr[count + 1];
    int r = 0;
    for (int i = 0; i < 256; i++)
        if (ascii[i].freq > 0)
            arr[r++] = ascii[i];
    arr[r].freq = INFINITY;

    int maxSize = 1;
    for (int i = 0; i < 256; i++)
        if ((int)log10(ascii[i].freq) + 1 > maxSize)
            maxSize = (int)log10(ascii[i].freq) + 1;
    LSD_sort(arr, count, maxSize, 10);
    //quicksort(arr, 0, count);

    Node tree = buildTree(arr, count);

    fillCodes(&tree, BinCode(), codes);
}

void writeTable(char *fileName, Node *ascii, short count) {
    ofstream file(fileName, ios::binary);
    if (!file)
        return;

    file.write((char*)(&count), sizeof(count));

    for (int i = 0; i < 256; i++)
        if (ascii[i].freq > 0) {
            file << (char)i;
            file.write((char*)(&ascii[i].freq), sizeof(Node::freq));
        }

    file.close();
}

fstream::pos_type fileSize(char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void writeData(char *inputName, char *outputName, BinCode *codes) {

    uint32_t bytesCount = fileSize(inputName);

    ifstream input(inputName, ios::binary);
    if (!input)
        return;
    ofstream output(outputName, ios::binary | ios::app);
    if (!output)
        return;

    output.write((char*)(&bytesCount), sizeof(bytesCount));

    BinCode bytes;
    while (!input.eof()) {
        unsigned char ch = input.get();
        if (input.fail())
            break;

        BinCode code = codes[ch];
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

    short count;
    Node ascii[256];
    for (int i = 0; i < 256; i++)
        ascii[i].ch =  i;

    readFile(fileName, ascii, count);

    BinCode codes[256];

    clock_t start, end;
    start = clock();

    staticHuffman(ascii, count, codes);

    end = clock();
    double seconds = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nThe time: " << seconds << " seconds\n";

    //for (int i = 0; i < 256; i++)
    //   if (codes[i].count > 0)
    //       cout << '|' << (int)i << '|' << codes[i] << '\n';

    writeTable("../Result", ascii, count);

    writeData(fileName, "../Result", codes);

    return 0;
}
