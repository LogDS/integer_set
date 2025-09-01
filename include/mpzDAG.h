/*
 * mpzDAG.h
 * This file is part of tricotomy
 *
 * Copyright (C) 2019 - Giacomo Bergami
 *
 *  tricotomy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  tricotomy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with  tricotomy. If not, see <http://www.gnu.org/licenses/>.
 */



//
// Created by giacomo on 08/12/2019.
//

#ifndef INTEGER_SETS_MPZDAG_H
#define INTEGER_SETS_MPZDAG_H

#include <gmp.h>
#include "BigIntegerForBitMask.h"
#include <vector>
#include <unordered_set>
#include <set>
#include "basic_operations.h"

/**
 * Node used within the dag to implement the sparse biginteger representation
 */
struct trichotomyNode {
    trichotomyNode* g_prod1;
    size_t pv;
    trichotomyNode* d_sum0;
    char is1Or2;
    char is1Or0;

    trichotomyNode(trichotomyNode *g, size_t p, trichotomyNode *d);
    trichotomyNode(bool is1Otherwise0);
    tricotomiaNode& operator=(const trichotomyNode& x);

    bool isZero() const;
    bool isOne() const;
    char compareWith(trichotomyNode *rhs) const;

    /**
     * @brief: interface to be mainly used when the pv is pretty low (e.g., 0). This is allowed to do fast arithmetic operations
     * @return  the (truncated) size_t representation of the value
     */
    size_t compute() const;

    /**
     * @brief: obtains the mpz_t representation of the big integer
     * @return
     */
    BigIntegerForBitMask computeBig() const;

    /**
     *
     * @param g     product part
     * @param p     power part
     * @param d     sum part
     * @return      biginteger representation of the number
     */
    static BigIntegerForBitMask computeBig(trichotomyNode*g, size_t p, trichotomyNode*d );

    bool operator<(const trichotomyNode &rhs) const;
    bool operator>(const trichotomyNode &rhs) const;
    bool operator<=(const trichotomyNode &rhs) const;
    bool operator>=(const trichotomyNode &rhs) const;
    bool operator==(const trichotomyNode &rhs) const;
    bool operator!=(const trichotomyNode &rhs) const;
};

/**
 * dag keeping track of all the numbers that have been generated
 */
struct  mpzDAG {
    std::unordered_map<BigIntegerForBitMask, trichotomyNode> innerDag;

    /**
   * This operation returns the graph representation of the given number
   * @param n
   * @param isAlreadySet if the value was already in the database or not
   * @return
   */
    trichotomyNode* get(const BigIntegerForBitMask& n);
    trichotomyNode* get(const size_t& n);

    trichotomyNode* tauConstructor(const BigIntegerForBitMask &n, const BigIntegerForBitMask &gv, const BigIntegerForBitMask &dv, const size_t &pv);
    trichotomyNode* tauConstructor(const BigIntegerForBitMask &gv, const BigIntegerForBitMask &dv, size_t &pv);

    /**
     *
     * @param a
     * @param b
     * @return
     */
    trichotomyNode* Or(trichotomyNode* a, trichotomyNode* b);

private:
    trichotomyNode * commonForTauConstructors(const BigIntegerForBitMask &n, trichotomyNode *g, const size_t &pv, trichotomyNode *d);
    trichotomyNode * commonForTauConstructors(trichotomyNode *g, const size_t &pv, trichotomyNode *d);
};

struct trichotomyEntry {
    size_t lastId;
    trichotomyNode* setPtr;

    trichotomyEntry(size_t lastId, trichotomyNode *setPtr);

    bool operator==(const trichotomyEntry &rhs) const;
    bool operator!=(const trichotomyEntry &rhs) const;
    bool operator<(const trichotomyEntry &rhs) const;
    bool operator>(const trichotomyEntry &rhs) const;
    bool operator<=(const trichotomyEntry &rhs) const;
    bool operator>=(const trichotomyEntry &rhs) const;
};

/**
 * @brief Computes the possible entries, with no deduplications.
 *
 * This function ensures the optimality of the path search from a given graph containing loops, by using the following
 * techniques:
 * - the paths that were already visited are discarded.
 * - As a consequence, all the already visited nodes reachable from discarded paths are also discarded.
 *
 * @param dag               DAG acting as the map of all the sets, providing the possible paths for the computation
 * @param entries           First entries of path size 1. Each of them contains which is the target relation to visit next
 * @param maxThreshold      Maximum number of iterations to undergo
 * @param newEntries        Navigation function expanding each relationship to all the possible outgoing reachable relationships
 * @return                  The computed paths
 */
std::set<trichotomyNode*> expansionAlgorithm(mpzDAG& dag, std::vector<trichotomyEntry>& entries, size_t maxThreshold, std::vector<trichotomyEntry> (*newEntries)(size_t id));

#endif //INTEGER_SETS_MPZDAG_H
