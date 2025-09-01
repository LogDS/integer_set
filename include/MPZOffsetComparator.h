/*
 * MPZOffsetComparator.h
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

#ifndef INTEGER_SETS_MPZOFFSETCOMPARATOR_H
#define INTEGER_SETS_MPZOFFSETCOMPARATOR_H

#include <rocksdb/db.h>
#include "RocksDBKeyValue.h"
#include "BigIntegerForBitMask.h"


/**
 * This comparator assumes that the elements within the RocksDB database are RocksDBKeyValue
 */
class MPZOffsetComparator : public rocksdb::Comparator {
    static char* val;
public:
    ~MPZOffsetComparator() override = default;

    // Three-way comparison function:
    // if a < b: negative result
    // if a > b: positive result
    // else: zero result
    int Compare(const rocksdb::Slice& a, const rocksdb::Slice& b) const override;

    // Ignore the following methods for now:
    const char* Name() const override;
    void FindShortestSeparator(std::string* /*start*/, const rocksdb::Slice& /*limit*/) const override;
    void FindShortSuccessor(std::string*) const override;

private:
    void FindShortSuccessor(BigIntegerForBitMask &left, std::string *str) const;
};

#endif //INTEGER_SETS_MPZOFFSETCOMPARATOR_H
