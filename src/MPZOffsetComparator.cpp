//
// Created by giacomo on 05/12/2019.
//

#include "MPZOffsetComparator.h"
#include "BigIntegerForBitMask.h"

char* MPZOffsetComparator::val = "MPZOffsetComparator";

const char *MPZOffsetComparator::Name() const {
    return val;
}

int MPZOffsetComparator::Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const {
    BigIntegerForBitMask left(a.size()-sizeof(RocksDBKeyValue), ((char*)a.data())+sizeof(RocksDBKeyValue));
    BigIntegerForBitMask right(b.size()-sizeof(RocksDBKeyValue), ((char*)b.data())+sizeof(RocksDBKeyValue));
    return left.compareWith(right);
}

void MPZOffsetComparator::FindShortestSeparator(std::string *str, const rocksdb::Slice &b) const {
    if (!str) return;
    BigIntegerForBitMask left(str->size()-sizeof(RocksDBKeyValue), ((char*)str->c_str())+sizeof(RocksDBKeyValue));
    BigIntegerForBitMask right(b.size()-sizeof(RocksDBKeyValue), ((char*)b.data())+sizeof(RocksDBKeyValue));
    if (left.compareWith(right) < 0) {
        FindShortSuccessor(left, str);
    }
}

void MPZOffsetComparator::FindShortSuccessor(std::string *str) const {
    if (!str) return;
    BigIntegerForBitMask left(str->size()-sizeof(RocksDBKeyValue), ((char*)str->c_str())+sizeof(RocksDBKeyValue));
    FindShortSuccessor(left, str);
}

void MPZOffsetComparator::FindShortSuccessor(BigIntegerForBitMask &left, std::string *str) const {
    auto x = (left+1);
    size_t min = x.serializedSize();
    size_t memtoAlloc = min + sizeof(RocksDBKeyValue);
    struct iovec toAlloc{malloc(memtoAlloc),memtoAlloc};
    ((RocksDBKeyValue*)toAlloc.iov_base)->offset = 0;
    str->clear();
    str->copy((char*)((RocksDBKeyValue*)toAlloc.iov_base)->mpz, min);
    if (toAlloc.iov_base != nullptr) free(toAlloc.iov_base);
}
