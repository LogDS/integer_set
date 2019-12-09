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
