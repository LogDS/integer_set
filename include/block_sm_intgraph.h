/*
 * block_sm_intgraph.h
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

#ifndef INTEGER_SETS_BLOCK_SM_INTGRAPH_H
#define INTEGER_SETS_BLOCK_SM_INTGRAPH_H

// Forward declaration
struct block_sm_pointer;

struct block_sm_intgraph {
    struct block_sm_pointer *sum, *power, *prod;
};

enum block_sm_base_cases {
    IS_ONE,
    IS_ZERO
};

struct block_sm_pointer {
    block_sm_base_cases baseCases;
    bool               isInSolutionFileOtherwiseNumAllocation;
    struct block_sm_intgraph* ptr;
};
#endif //INTEGER_SETS_BLOCK_SM_INTGRAPH_H
