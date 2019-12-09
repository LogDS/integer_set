//
// https://www.di.ens.fr/~jv/HomePage/pdf/dichotomy09.pdf
//

#ifndef INTEGER_SETS_BASIC_OPERATIONS_H
#define INTEGER_SETS_BASIC_OPERATIONS_H

#include <cmath>
#include <ostream>
#include <unordered_map>
#include <iostream>

template <typename Numeric> static inline Numeric l(Numeric value) {
    if (value <= 0)
        return 0;
    else
        return ((Numeric)1) +  (Numeric)std::floor(std::log2(value));
}

template <typename Numeric> static inline Numeric ll(Numeric value) {
    if (value < 2)
        return 0;
    else
        return l<Numeric>(l<Numeric>(value)-1);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
 * This class only performs the computation for initializing the number as expected.
 * @tparam Numeric
 */
template <typename Numeric> struct tricotomia {
    Numeric g;
    Numeric p;
    Numeric d;
    char is1Or2;
    char is1Either2;

    tricotomia(Numeric g, Numeric p, Numeric d) : g(g), p(p), d(d), is1Or2{0}, is1Either2{0}
    {
    }

    tricotomia(Numeric n) {
        if (n <= 1) {
            is1Or2 = true;
            is1Either2 = n <= 1;
        } else {
            is1Or2 = false;
            p = ll<Numeric>(n)-(Numeric)1;
            Numeric tmp = std::pow(2, std::pow(2, p));
            g = n / tmp;
            d = n % tmp;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const tricotomia &tricotomia) {
        os << "molto: " << tricotomia.g << " p: " << tricotomia.p << " summato: " << tricotomia.d;
        os << " test: " << (tricotomia.d+std::pow(2, std::pow(2, tricotomia.p))*tricotomia.g);
        return os;
    }
};

struct tricotomiaNode {
    tricotomiaNode* g_prod1;
    tricotomiaNode* p;
    tricotomiaNode* d_sum0;
    char is1Or2;
    char is1Or0;

    tricotomiaNode(tricotomiaNode *g, tricotomiaNode *p, tricotomiaNode *d, char is1Or2, char is1Either2);
    tricotomiaNode& operator=(const tricotomiaNode& x);
    tricotomiaNode(const tricotomiaNode& x);

    bool isZero() const {
        return (!is1Or2) && is1Or0;
    }

    bool isOne() const {
        return is1Or2 && is1Or0;
    }

    /**
     * @brief: interface for debugging purposes (printing the resulting number)
     * @tparam Numeric
     * @return
     */
    template <typename Numeric> Numeric compute() const {
        if (is1Or2) {
            return is1Or0 ? 1 : 2;
        } else {
            if (is1Or0) return 0;
            Numeric dv =  d_sum0->compute<Numeric>();
            Numeric pv =  p->compute<Numeric>();
            Numeric gv =  g_prod1->compute<Numeric>();
            Numeric result = (dv+((Numeric)std::pow(2, std::pow(2, pv)))*gv);

            return result;
        }
    }


    /*template <typename Numeric>*/ char compareWith(tricotomiaNode* rhs) const;

    bool operator==(const tricotomiaNode& rhs) const;

    bool operator<(const tricotomiaNode& rhs) const;

    /**
     * Printing for debugging purposes
     */
    template <typename Numeric> void print() const {
        if (isOne())
            std::cout << "1" << std::endl;
        /*else if (isTwo())
            std::cout << "2" << std::endl;
        */else if (isZero())
            std::cout << "0" << std::endl;
        else {
        Numeric dv =  d_sum0->compute<Numeric>();
        Numeric pv =  p->compute<Numeric>();
        Numeric gv =  g_prod1->compute<Numeric>();
        Numeric result = (dv+((Numeric)std::pow(2, std::pow(2, pv)))*gv);
        std::cout << "d: " << dv << " p: " <<  pv << " g: " << gv  << " ==> " << result << std::endl;
        }
    }
};

template <typename Numeric> struct dag {
    std::unordered_map<Numeric, tricotomiaNode>  innerDag;


    dag() {
        innerDag.insert(std::make_pair(0, tricotomiaNode{nullptr, nullptr, nullptr, 0, 1}));
        innerDag.insert(std::make_pair(1, tricotomiaNode{nullptr, nullptr, nullptr, 1, 1}));
        //innerDag.insert(std::make_pair(2, tricotomiaNode{nullptr, nullptr, nullptr, 1, 0}));
    }

    /**
     * This operation returns the graph representation of the given number
     * @param n
     * @return
     */
    tricotomiaNode* get(Numeric n) {
        typename std::unordered_map<Numeric, tricotomiaNode>::iterator ptr = innerDag.find(n);
        if (ptr != innerDag.end()) return &ptr->second;
        else {
            tricotomia<Numeric> tmp{n};
            tricotomiaNode* g = get(tmp.g);
            tricotomiaNode* d = get(tmp.d);
            tricotomiaNode* p = get(tmp.p);
            return &innerDag.insert(std::make_pair(n, tricotomiaNode{g, p, d, 0, 0})).first->second;
        }
    }

    /**
     * This operation decrements the current number
     * @param lhs
     * @return
     */
    tricotomiaNode* decrement(tricotomiaNode* lhs) {
        if (lhs->p == nullptr && lhs->g_prod1 == nullptr && lhs->d_sum0 == nullptr) {
            Numeric n = lhs->compute<Numeric>();
            if (n <= 2)
                return get(n-1);
        }
        if (!lhs->d_sum0->isZero()) {
            tricotomiaNode* d = decrement(lhs->d_sum0);
            Numeric dv =  d->compute<Numeric>();
            Numeric pv =  lhs->p->compute<Numeric>();
            Numeric gv =  lhs->g_prod1->compute<Numeric>();
            Numeric n = (dv+((Numeric)std::pow(2, std::pow(2, pv)))*gv);
            return &innerDag.insert(std::make_pair(n, tricotomiaNode{lhs->g_prod1, lhs->p, decrement(lhs->d_sum0), 0, 0})).first->second;
        } else {
            tricotomiaNode* g = decrement(lhs->g_prod1);
            Numeric gv =  g->compute<Numeric>();
            tricotomiaNode* d = xp(lhs->p);
            Numeric dv =  d->compute<Numeric>();
            Numeric pv =  lhs->p->compute<Numeric>();
            Numeric n = (dv+((Numeric)std::pow(2, std::pow(2, pv)))*gv);
            return &innerDag.insert(std::make_pair(n, tricotomiaNode{g, lhs->p, d, 0, 0})).first->second;
        }
    }


    /**
     * This operation increments the current number
     * @param lhs
     * @return
     */
    tricotomiaNode* increment(tricotomiaNode* lhs) {
        if (lhs->p == nullptr && lhs->g_prod1 == nullptr && lhs->d_sum0 == nullptr) {
            Numeric n = lhs->compute<Numeric>();
            if (n <= 2)
                return get(n+1);
        }
        tricotomiaNode* xpp = xp(lhs->p);
        if (lhs->d_sum0->compareWith(xpp) != 0) {
            tricotomiaNode* d = increment(lhs->d_sum0);
            return tauConstructor(lhs->g_prod1, lhs->p, d);

            /*Numeric dv =  d->compute<Numeric>();
            Numeric gv =  lhs->g_prod1->compute<Numeric>();
            Numeric pv =  lhs->p->compute<Numeric>();
            Numeric n = (dv+((Numeric)std::pow(2, std::pow(2, pv)))*gv);
            return &innerDag.insert(std::make_pair(n, tricotomiaNode{lhs->g_prod1, lhs->p, d, 0, 0})).first->second;
        */} else if (lhs->g_prod1->compareWith(xpp) != 0) {

            tricotomiaNode* d = get(0);
            tricotomiaNode* g = increment(lhs->g_prod1);

            return tauConstructor(g, lhs->p, d);

            /*Numeric pv =  lhs->p->compute<Numeric>();
            Numeric gv =  g->compute<Numeric>();
            Numeric n = (((Numeric)std::pow(2, std::pow(2, pv)))*gv);
            return &innerDag.insert(std::make_pair(n, tricotomiaNode{g, lhs->p, d, 0, 0})).first->second;
        */} else {
            tricotomiaNode* p = increment(lhs->p);
            return tauConstructor(get(1), p, get(0));
            //return &innerDag.insert(std::make_pair(((Numeric)std::pow(2, std::pow(2, p->compute<Numeric>()))), tricotomiaNode{get(1), p, get(0), 0, 0})).first->second;
        }
    }

    tricotomiaNode* tauConstructor(tricotomiaNode* g, tricotomiaNode* p, tricotomiaNode* d) {
        Numeric gv = g->compute<Numeric>();
        Numeric pv = p->compute<Numeric>();
        Numeric dv = d->compute<Numeric>();
        Numeric n = (dv+((Numeric)std::pow(2, std::pow(2, pv)))*gv);
        return &innerDag.insert(std::make_pair(n, tricotomiaNode{g, p, d, 0, 0})).first->second;

    }

    tricotomiaNode* Constructor(tricotomiaNode* g, tricotomiaNode* p, tricotomiaNode* d) {
        char result =p->compareWith(g->p);
        if (result > 0)
            return C1(g, p, d);
        else if (result == 0)
            return C1(g->d_sum0, p, A(d, g->g_prod1));
        else
            return Constructor(Constructor(g->d_sum0, p, d),g->p, g->g_prod1);
    }

    tricotomiaNode* And(tricotomiaNode* a, tricotomiaNode* b) {
        if (a->isZero()) {
            return get(0);
        }  /*else if (a->isTwo()) {
            return get(b->compute<Numeric>() & 2);
        }*/ else if (a == b) {
            return a;
        } else if (a->compareWith(b) > 0) {
            return And(b, a);
        } else if (a->isOne()) {
            if (b->isOne())
                return a;
            else if (b->isZero())
                return b;
            else if (b->p->compareWith(get(0)) == 0)
                // a is jut one, so if the and returns zero, all the other elements should be zero, too
                if (get(b->d_sum0->compute<Numeric>() & 1)->isOne()) return a; else return get(0);
            else
                tauConstructor(b->g_prod1, b->p, And(a->d_sum0, b->d_sum0));
        } else if (a->p->compareWith(b->p) < 0) {
            return And(a, b->d_sum0);
        } else {
            return tauConstructor(And(a->g_prod1, b->g_prod1), a->p, And(a->d_sum0, b->d_sum0));
        }
    }

    tricotomiaNode* Or(tricotomiaNode* a, tricotomiaNode* b) {
        if (a->isZero()) {
            return b;
        }  /*else if (a->isTwo()) {
            return get(b->compute<Numeric>() & 2);
        }*/ else if (a == b) {
            return a;
        } else if (a->compareWith(b) > 0) {
            return Or(b, a);
        } else if (a->isOne()) {
            if (b->isZero() || b->isOne())
                return a;
            else if (b->p->compareWith(get(0)) == 0)
                return tauConstructor(b->g_prod1, b->p,get(b->d_sum0->compute<Numeric>() | 1));
            else
                tauConstructor(b->g_prod1, b->p, Or(a, b->d_sum0));
        } else if (a->p->compareWith(b->p) < 0) {
            return tauConstructor(b->g_prod1, b->p, Or(a, b->d_sum0));
        } else {
            return tauConstructor(Or(a->g_prod1, b->g_prod1), a->p, Or(a->d_sum0, b->d_sum0));
        }
    }

    tricotomiaNode* twice(tricotomiaNode* t) {
        if (t->isZero())
            return t;
        else if (t->isOne())
            return get(2);
        else
            return Constructor(twice(t->g_prod1), t->p, twice(t->d_sum0));
    }

private:
    tricotomiaNode* xp(tricotomiaNode* lhs) {
        if (lhs->isZero()) {
            return get(1);
        } else {
            tricotomiaNode* q = decrement(lhs);
            tricotomiaNode* a = xp(q);
            Numeric av = a->compute<Numeric>();
            Numeric qv = q->compute<Numeric>();
            Numeric n = (av+((Numeric)std::pow(2, std::pow(2, qv)))*av);
            return &innerDag.insert(std::make_pair(n, tricotomiaNode{a, q, a, 0, 0})).first->second;
        }
    }

    tricotomiaNode* A(tricotomiaNode* a, tricotomiaNode* b) {
        if (a->isZero()) {
            return b;
        } else if (a->isOne()) {
            return increment(b);
        } /*else if (a->isTwo()) {
            return increment(increment(b));
        }*/ else if (a == b) {
            return twice(a);
        } else if (a->compareWith(b) > 0) {
            return A(b,a);
        } else if (a->p->compareWith(b->p) < 0) {
            return Constructor(A(a, b->d_sum0), b->p, b->g_prod1);
        } else
            return Am(a->g_prod1, b->g_prod1);

    }

    tricotomiaNode* C1(tricotomiaNode* g, tricotomiaNode* p, tricotomiaNode* d) {
        char comparison = p->compareWith(d->p);
        if ((d->d_sum0 == nullptr && d->g_prod1 == nullptr && d->p == nullptr) || (comparison > 0)) {
            return tauConstructor(g, p, d);
        } else if (comparison == 0) {
            return tauConstructor(tauConstructor(g, p, d->d_sum0), increment(p), d->g_prod1);
        } else {
            return Constructor(C1(g, p, d->d_sum0), d->p, C1(get(0), p, d->g_prod1));
        }
    }

    tricotomiaNode* Am(tricotomiaNode* a, tricotomiaNode* b) {
        return Constructor(A(a->d_sum0, b->d_sum0), a->p, A(a->g_prod1, b->g_prod1));
    }
};

#endif //INTEGER_SETS_BASIC_OPERATIONS_H
