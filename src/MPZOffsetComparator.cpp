/*
 * MPZOffsetComparator.cpp
 * This file is part of tricotomy
 *
 * Copyright (C) 2019 - Giacomo Bergami
 *
 * tricotomy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * tricotomy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tricotomy. If not, see <http://www.gnu.org/licenses/>.
 */

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
