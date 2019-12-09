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
