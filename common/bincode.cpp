#include "bincode.h"

BinCode::BinCode() {
    count = 0;
    code = 0;
}

BinCode::BinCode(int64_t n) {
    if (n == 0)
        count = 1;
    else
        count = 0;
    code = n;
    for (int i = 0; i < 64; i++)
        if ((code >> i) & 1)
            count = i + 1;
}

BinCode BinCode::operator>>(int n) {
    if (n < 0)
        exit(1);

    BinCode res = *this;
    if (n >= count)
        res.count = 0;
    else
        res.count -= n;
    res.code >>= n;
    return res;
}

BinCode BinCode::operator>>=(int n) {
    *this = *this >> n;
    return *this;
}

BinCode BinCode::operator<<(int n) {
    if (n < 0)
        exit(1);
    BinCode res = *this;
    if (n + count >= 64)
        res.count = 64;
    else
        res.count += n;
    res.code <<= n;
    return res;
}

BinCode BinCode::operator<<=(int n) {
    *this = *this << n;
    return *this;
}

BinCode BinCode::operator|(BinCode binCode) {
    BinCode res = *this;
    res.code |= binCode.code;
    return res;
}

BinCode BinCode::operator|=(BinCode binCode) {
    *this = *this | binCode;
    return *this;
}

ostream& operator<<(ostream& os, const BinCode& binCode) {
    for (int i = binCode.count - 1; i >= 0; i--)
        os << ((binCode.code >> i) & 1);
    return os;
}
