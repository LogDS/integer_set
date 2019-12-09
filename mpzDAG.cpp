//
// Created by giacomo on 08/12/2019.
//

#include "mpzDAG.h"

/*
bool mpzEquals::operator()(const __mpz_struct *val1, const __mpz_struct *val2) const {
    return mpz_cmp(val1, val2) == 0;
}

size_t hash_mpz_t::operator()(const mpz_t &x) const {
    return x[0]._mp_size != 0 ? static_cast<size_t>(x[0]._mp_d[0]) : 0;
}*/

trichotomyNode *mpzDAG::get(const BigIntegerForBitMask &n) {
    auto ptr = innerDag.find(n);
    if (ptr != innerDag.end()) {
        return &ptr->second;
    }
    else {
        mpz_t sum, prod;
        size_t p;
        mpz_init(sum); mpz_init(prod);
        n.prepareForBitDAG(prod, p, sum);
        BigIntegerForBitMask gv = BigIntegerForBitMask(prod, 0);
        BigIntegerForBitMask dv = BigIntegerForBitMask(sum, 0);
        return tauConstructor(n, gv, dv, p);
    }
}

trichotomyNode *
mpzDAG::tauConstructor(const BigIntegerForBitMask &n, const BigIntegerForBitMask &gv, const BigIntegerForBitMask &dv,
                       const size_t &pv) {
    bool ignore;
    trichotomyNode* g = get(gv);
    trichotomyNode* d = get(dv);
    return commonForTauConstructors(n, g, pv, d);
}

trichotomyNode *
mpzDAG::commonForTauConstructors(const BigIntegerForBitMask &n, trichotomyNode *g, const size_t &pv, trichotomyNode *d) {
    auto result = innerDag.insert(std::make_pair(n, trichotomyNode{g, pv, d}));
    return &result.first->second;
}


trichotomyNode *mpzDAG::get(const size_t &n) {
    trichotomyNode *element = nullptr;
    {
        mpz_t local;
        mpz_init(local);
        mpz_set_ui(local, n);
        BigIntegerForBitMask locale{local, 0};
        element = get(locale);
    }
    return element;
}

trichotomyNode *mpzDAG::tauConstructor(const BigIntegerForBitMask &gv, const BigIntegerForBitMask &dv, size_t &pv) {
    bool ignore;
    trichotomyNode* g = get(gv);
    trichotomyNode* d = get(dv);
    BigIntegerForBitMask n = gv.timesExpExp2(pv) + dv;
    return commonForTauConstructors(n, g, pv, d);
}

trichotomyNode *mpzDAG::commonForTauConstructors(trichotomyNode *g, const size_t &pv, trichotomyNode *d) {
    BigIntegerForBitMask n = trichotomyNode::computeBig(g, pv, d);
    return commonForTauConstructors(n, g, pv, d);
}

trichotomyNode *mpzDAG::Or(trichotomyNode *a, trichotomyNode *b) {
    bool ignore;
    if (a->isZero()) {
        return b;
    } else if (a == b) {
        return a;
    } else if (a->compareWith(b) > 0) {
        return Or(b, a);
    } else if (a->isOne()) {
        if (b->isZero() || b->isOne())
            return a;
        else if (b->pv == 0) {
            BigIntegerForBitMask n = BigIntegerForBitMask::generate(b->compute() | 1);
            return commonForTauConstructors(n, b->g_prod1, b->pv,get(b->d_sum0->compute() | 1));
        } else {
            commonForTauConstructors(b->g_prod1, b->pv, Or(a, b->d_sum0));
        }
    } else if (a->pv < b->pv) {
        return commonForTauConstructors(b->g_prod1, b->pv, Or(a, b->d_sum0));
    } else {
        return commonForTauConstructors(Or(a->g_prod1, b->g_prod1), a->pv, Or(a->d_sum0, b->d_sum0));
    }
}

bool trichotomyEntry::operator==(const trichotomyEntry &rhs) const {
    return lastId == rhs.lastId &&
            ((setPtr == nullptr && rhs.setPtr == nullptr) || (*setPtr == *rhs.setPtr));
}

bool trichotomyEntry::operator!=(const trichotomyEntry &rhs) const {
    return !(rhs == *this);
}

bool trichotomyEntry::operator<(const trichotomyEntry &rhs) const {
    if (setPtr == nullptr && rhs.setPtr == nullptr) {
        return lastId < rhs.lastId;
    } else if (setPtr == nullptr) {
        return true;
    } else if (rhs.setPtr == nullptr) {
        return false;
    } else {
        auto cmp = setPtr->compareWith(rhs.setPtr);
        return ((cmp < 0) || (cmp == 0 && lastId < rhs.lastId));
    }
}

bool trichotomyEntry::operator>(const trichotomyEntry &rhs) const {
    return rhs < *this;
}

bool trichotomyEntry::operator<=(const trichotomyEntry &rhs) const {
    return !(rhs < *this);
}

bool trichotomyEntry::operator>=(const trichotomyEntry &rhs) const {
    return !(*this < rhs);
}

trichotomyEntry::trichotomyEntry(size_t lastId, trichotomyNode *setPtr) : lastId(lastId), setPtr(setPtr) {}

trichotomyNode::trichotomyNode(trichotomyNode *g, size_t p, trichotomyNode *d) : g_prod1{g}, pv{p}, d_sum0{d}, is1Or0{0}, is1Or2{0} {}

trichotomyNode::trichotomyNode(bool is1Otherwise0) : g_prod1{nullptr}, pv{0}, d_sum0{nullptr} {
    is1Or0 = 1;
    is1Or2 = is1Otherwise0 ? 1 : 0;
}

tricotomiaNode &trichotomyNode::operator=(const trichotomyNode &x) {
    g_prod1 = x.g_prod1;
    pv = x.pv;
    d_sum0 = x.d_sum0;
    is1Or2 = x.is1Or2;
    is1Or0 = x.is1Or0;
}

bool trichotomyNode::operator<(const trichotomyNode &rhs) const {
    return compareWith((trichotomyNode*)&rhs) < 0;
}

bool trichotomyNode::operator>(const trichotomyNode &rhs) const {
    return rhs < *this;
}

bool trichotomyNode::operator<=(const trichotomyNode &rhs) const {
    return !(rhs < *this);
}

bool trichotomyNode::operator>=(const trichotomyNode &rhs) const {
    return !(*this < rhs);
}

bool trichotomyNode::operator==(const trichotomyNode &rhs) const {
    return compareWith((trichotomyNode*)&rhs) == 0;
}

bool trichotomyNode::operator!=(const trichotomyNode &rhs) const {
    return !(rhs == *this);
}

bool trichotomyNode::isZero() const {
    return (!is1Or2) && is1Or0;
}

bool trichotomyNode::isOne() const {
    return is1Or2 && is1Or0;
}

char trichotomyNode::compareWith(trichotomyNode *rhs) const {
    if (rhs != nullptr) {
        if (this == rhs)
            return 0;
        if (is1Or0) {
            if (rhs->is1Or0) {
                char ll = isZero() ? 0 : 1;
                char rl = rhs->isZero() ? 0 : 1;
                return (ll < rl) ? (char)-1 : (ll == rl ? (char)0 : (char)1);
            } else {
                return -1;
            }
        } else if (rhs->is1Or0) {
            return 1;
        } else if (this->pv != rhs->pv)
            return pv < rhs->pv ? (char)-1 : (pv == rhs->pv ? (char)0 : (char)1);
        else if (this->g_prod1 != (rhs->g_prod1))
            return this->g_prod1->compareWith(rhs->g_prod1);
        else
            return this->d_sum0->compareWith(rhs->d_sum0);
    }
    return 1;
}

size_t trichotomyNode::compute() const {
    if (is1Or2) {
        return is1Or0 ? 1 : 2;
    } else {
        if (is1Or0) return 0;
        size_t dv =  d_sum0->compute();
        size_t gv =  g_prod1->compute();
        size_t result = (dv+((size_t)std::pow(2, std::pow(2, pv)))*gv);
        return result;
    }
}

BigIntegerForBitMask trichotomyNode::computeBig() const {
    if (is1Or2) {
        return is1Or0 ? BigIntegerForBitMask::one : BigIntegerForBitMask::two;
    } else {
        if (is1Or0) return BigIntegerForBitMask::zero;
        return trichotomyNode::computeBig(g_prod1, pv, d_sum0);
    }
}

BigIntegerForBitMask trichotomyNode::computeBig(trichotomyNode *g, size_t p, trichotomyNode *d) {
    BigIntegerForBitMask dv =  d->computeBig();
    BigIntegerForBitMask gv =  g->computeBig();
    return gv.timesExpExp2(p) + dv;
}
