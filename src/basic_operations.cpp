//
// Created by giacomo on 01/12/2019.
//

#include "basic_operations.h"

tricotomiaNode::tricotomiaNode(tricotomiaNode *g, tricotomiaNode *p, tricotomiaNode *d, char is1Or2, char is1Either2)
        : g_prod1(g), p(p), d_sum0(d), is1Or2(is1Or2), is1Or0(is1Either2) {}

tricotomiaNode &tricotomiaNode::operator=(const tricotomiaNode &x) {
    g_prod1 = x.g_prod1;
    p = x.p;
    d_sum0 = x.d_sum0;
    is1Or2 = x.is1Or2;
    is1Or0 = x.is1Or0;
}

tricotomiaNode::tricotomiaNode(const tricotomiaNode &x) : g_prod1{x.g_prod1}, p{x.p}, d_sum0{x.d_sum0}, is1Or2{x.is1Or2}, is1Or0{x.is1Or0} {}

bool tricotomiaNode::operator==(const tricotomiaNode &rhs) const {
    return (compareWith((tricotomiaNode*)&rhs) == 0);
}

bool tricotomiaNode::operator<(const tricotomiaNode &rhs) const {
    return (compareWith((tricotomiaNode*)&rhs) < 0);
}

char tricotomiaNode::compareWith(tricotomiaNode *rhs) const {
    if (rhs != nullptr) {
        if (this == rhs)
            return 0;
        if (p == nullptr && g_prod1 == nullptr && d_sum0 == nullptr) {
            if (rhs->p == nullptr && rhs->g_prod1 == nullptr && rhs->d_sum0 == nullptr) {
                auto ll = compute<unsigned int>();
                auto rl = rhs->compute<unsigned int>();
                return (ll < rl) ? (char)-1 : (ll == rl ? (char)0 : (char)1);
            } else {
                return -1;
            }
        } else if (rhs->p == nullptr && rhs->g_prod1 == nullptr && rhs->d_sum0 == nullptr) {
            return 1;
        } else if (this->p != rhs->p)
            return this->p->compareWith(rhs->p);
        else if (this->g_prod1 != (rhs->g_prod1))
            return this->g_prod1->compareWith(rhs->g_prod1);
        else
            return this->d_sum0->compareWith(rhs->d_sum0);
    }
    return 1;
}
