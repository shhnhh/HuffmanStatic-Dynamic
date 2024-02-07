#ifndef BINCODE_H
#define BINCODE_H

#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

class BinCode
{
public:
    uint8_t count;
    uint64_t code;

    BinCode();
    BinCode(int64_t);

    BinCode operator>>(int);
    BinCode operator>>=(int);
    BinCode operator<<(int);
    BinCode operator<<=(int);
    BinCode operator|(BinCode);
    BinCode operator|=(BinCode);
    friend ostream& operator<<(ostream&, const BinCode&);
};

#endif // BINCODE_H
