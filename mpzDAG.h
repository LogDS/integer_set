//
// Created by giacomo on 08/12/2019.
//

#ifndef INTEGER_SETS_MPZDAG_H
#define INTEGER_SETS_MPZDAG_H

#include <gmp.h>
#include <BigIntegerForBitMask.h>
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
std::set<trichotomyNode*> expansionAlgorithm(mpzDAG& dag, std::vector<trichotomyEntry>& entries, size_t maxThreshold, std::vector<trichotomyEntry> (*newEntries)(size_t id)) {
    std::vector<trichotomyEntry> current{entries};

    // Getting all the possible non-redundant paths to return as results
    std::set<trichotomyNode*> results;

    // Setting the initial entries as initial paths of length
    for (const trichotomyEntry& e : entries)
        results.emplace(e.setPtr);

    // If the length of the path is 1, then we've already got the relationships of interest
    while (maxThreshold != 1) {
        std::set<trichotomyEntry> ptrSet;// todo: unordered set. I need to implement the hash function
        for (const trichotomyEntry& e : current) {
            for (const trichotomyEntry& resultEntry: newEntries(e.lastId)) {
                trichotomyNode *resultingSet = dag.Or(e.setPtr, resultEntry.setPtr);
                if (results.insert(resultingSet).second) { // Checking whether we've already inserted such pointer
                    ptrSet.emplace(resultEntry.lastId, resultingSet); // If not, insert such into the next elements to be expanded
                }results
            }
        }

        if (ptrSet.empty()) { // If I met no further new path to expand, I can quit the iteration
            return results; // entries will contain the overall computed possible paths
        } else {
            // Setting up the elements to be iterated into a vector, so that the iteration can be quicker
            current.clear();
            current.insert(current.begin(), ptrSet.begin(), ptrSet.end());
        }
        // Countdowning the remaining allowed expansions to be computed
        maxThreshold--;
    }
    return results; // entries will contain the overall computed possible paths
}

#endif //INTEGER_SETS_MPZDAG_H
