//
// Created by giacomo on 02/12/2019.
//

#ifndef INTEGER_SETS_BIGINTEGERFORBITMASK_H
#define INTEGER_SETS_BIGINTEGERFORBITMASK_H

#include <gmp.h>
#include <bits/types/struct_iovec.h>
#include <cstdlib>
#include "basic_operations.h"

// TODO: if I know that the highest number to be rendered is x then it means that I shall generate all the 2^2 ... 2^i where i is the maximum bit

/**
 * This class allows to represent the number t as 2^t. This is done by setting the t-th bit to 1
 */
class BigIntegerForBitMask {
    unsigned long next;

public:
    __mpz_struct * integ;
    static BigIntegerForBitMask one;
    static BigIntegerForBitMask zero;
    static BigIntegerForBitMask two;

    /**
     * Default constructor with the elements that are required
     * @param integ
     * @param next
     */
    BigIntegerForBitMask(const __mpz_struct *integ, unsigned long next);

    static BigIntegerForBitMask generate(unsigned long i) {
        mpz_t init;
        mpz_init(init);
        mpz_set_ui(init, i);
        return BigIntegerForBitMask{init, 0};
    }

    static BigIntegerForBitMask expexp2(size_t i) {
        mpz_t init;
        mpz_init(init);
        mpz_ui_pow_ui(init, 2, std::pow(2, i));
        return BigIntegerForBitMask{init, 0};
    }

    /**
     * Initializes the BigIngeter using the ll numer, that is going to be represented as a 2^ll for bitmasking purposes
     * This is also setting the next element to be analysed accordingly to the Bit-DAG representation
     *
     * @param ll    initializing number for the bitmap, that is the bit to be setted
     */
    BigIntegerForBitMask(unsigned long ll);

    /**
     * Copying for the big integers
     * @param copy
     */
    BigIntegerForBitMask(const BigIntegerForBitMask& copy);

    /**
     * Assignment operator
     * @param copy
     * @return
     */
    BigIntegerForBitMask& operator=(const BigIntegerForBitMask& copy);

    /**
     * Adding a number to the current value
     * @param i
     * @return
     */
    BigIntegerForBitMask operator+(unsigned int i);
    BigIntegerForBitMask operator-(unsigned int i);
    BigIntegerForBitMask operator+(const BigIntegerForBitMask& i) const;
    BigIntegerForBitMask timesExpExp2(size_t i) const {
        __mpz_struct * toRet = nullptr;
        mpz_init(toRet);
        mpz_mul(toRet, integ, expexp2(i).integ);
        return BigIntegerForBitMask{toRet, 0};
    }

    /**
     * Constructor importing the number from some serialized representation
     * @param size
     * @param ptr
     */
    BigIntegerForBitMask(size_t size, void* ptr);

    /**
     * Constructor importing the number from some serialized representation
     * @param ptr
     */
    BigIntegerForBitMask(struct iovec* ptr);

    /**
     * How much size will it take to serialize the given number in secondary memory
     * @return
     */
    size_t serializedSize();

    /**
     * Generating the object that needs to be written outside
     * @param out
     * @return
     */
    bool copy(struct iovec& out);

    // This will be used for the p part of the number
    size_t getMostSignificativeBit() const;

    size_t ll() const;

    BigIntegerForBitMask getDNumber() {
        return BigIntegerForBitMask(next);
    }

    ~BigIntegerForBitMask() {
        mpz_clear (integ);
    }

    int compareWith(const BigIntegerForBitMask& x) {
        return mpz_cmp(integ, x.integ);
    }

    /**
     *
     * @param mul  -- already initialized mpz_t
     * @param p    -- exponent value
     * @param add  -- already initialized mpz_t
     * @return
     */
    char prepareForBitDAG(mpz_t& mul, size_t& p, mpz_t& add) const {
        int cmp1 = mpz_cmp_ui(integ, 1);
        if (cmp1 == 0)
            return 1;
        cmp1 = mpz_cmp_ui(integ, 0);
        if (cmp1 == 0)
            return 0;
        p = ll()-1;
        unsigned long long x = std::pow(2ULL, p);
        mpz_mod_2exp(add, integ, x);
        mpz_div_2exp(mul, integ, x);
        return -1;
    }

    bool operator==(const BigIntegerForBitMask& rhs) const;
};

namespace std {
    template <> struct hash<BigIntegerForBitMask>
    {
        size_t operator()(const BigIntegerForBitMask& x) const
        {
            return x.integ[0]._mp_size != 0 ?
                   static_cast<size_t>(x.integ[0]._mp_d[0]) : 0;
        }
    };
}

#endif //INTEGER_SETS_BIGINTEGERFORBITMASK_H
