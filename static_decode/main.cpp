#include "../common/bincode.h"
#include "../common/node.h"
#include <cmath>

using namespace std;

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

Node createTree(Node *arr1, int count) {

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

void staticHuffman(Node *ascii, short count, Node& root) {

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

    root = createTree(arr, count);
}

void readTable(char *fileName, Node *ascii, short& count) {
    ifstream file(fileName, ios::binary);
    if (!file)
        return;

    count = 0;
    file.read((char*)(&count), sizeof(count));

    for (int i = 0; i < count; i++) {
        unsigned char ch = file.get();
        unsigned int freq;
        file.read((char*)(&freq), sizeof(freq));
        ascii[ch].freq = freq;
    }

    file.close();
}

void readData(char *inputName, char *outputName, Node& root) {

    ifstream input(inputName, ios::binary);
    if (!input)
        return;
    ofstream output(outputName, ios::binary);
    if (!output)
        return;

    short count = 0;
    input.read((char*)(&count), sizeof(count));

    input.seekg(count * (sizeof(Node::ch) + sizeof(Node::freq)), ios_base::cur);

    uint32_t bytesCount = 0;
    input.read((char*)(&bytesCount), sizeof(bytesCount));

    Node *node = &root;
    while (!input.eof()) {
        unsigned char ch = input.get();
        if (input.fail())
            break;
        for (int i = 7; i >= 0; i--) {
            bool bit = (ch >> i) & 1;
            if (bit == 0)
                node = node->left;
            else
                node = node->right;
            if (node->left == nullptr && node->right == nullptr) {
                output.write((char*)&(node->ch), 1);
                node = &root;
                bytesCount--;
                if (bytesCount == 0)
                    break;
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

    short count;
    Node ascii[256];
    for (int i = 0; i < 256; i++)
        ascii[i].ch =  i;

    readTable("../Result", ascii, count);

    Node root;

    clock_t start, end;
    start = clock();

    staticHuffman(ascii, count, root);

    end = clock();
    double seconds = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nThe time: " << seconds << " seconds\n";

    readData("../Result", "../Reverse", root);

    return 0;
}
