/*
 * RocksDBKeyValue.h
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

#ifndef INTEGER_SETS_ROCKSDBKEYVALUE_H
#define INTEGER_SETS_ROCKSDBKEYVALUE_H

#include <cstring>

struct RocksDBKeyValue {
    size_t offset; /// Offset wthin the file
    char   mpz[];  ///mpz_t in a sequential default format
};


#endif //INTEGER_SETS_ROCKSDBKEYVALUE_H
