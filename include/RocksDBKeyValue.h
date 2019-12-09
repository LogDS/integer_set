//
// Created by giacomo on 05/12/2019.
//

#ifndef INTEGER_SETS_ROCKSDBKEYVALUE_H
#define INTEGER_SETS_ROCKSDBKEYVALUE_H

#include <cstring>

struct RocksDBKeyValue {
    size_t offset; /// Offset wthin the file
    char   mpz[];  ///mpz_t in a sequential default format
};


#endif //INTEGER_SETS_ROCKSDBKEYVALUE_H
