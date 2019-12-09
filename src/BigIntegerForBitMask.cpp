//
// Created by giacomo on 02/12/2019.
//

#include "BigIntegerForBitMask.h"

BigIntegerForBitMask BigIntegerForBitMask::one = BigIntegerForBitMask::generate(1);
BigIntegerForBitMask BigIntegerForBitMask::zero = BigIntegerForBitMask::generate(0);
BigIntegerForBitMask BigIntegerForBitMask::two = BigIntegerForBitMask::generate(2);

BigIntegerForBitMask::BigIntegerForBitMask(const __mpz_struct *integ, unsigned long next) : integ(( __mpz_struct *)integ), next(next) {

}

BigIntegerForBitMask::BigIntegerForBitMask(unsigned long ll) {
    mpz_init(integ);
    mpz_setbit(integ, ll);
    next = ll & ~(1UL << __builtin_clzl(ll));
    // number &= ~(1UL << n);
}

BigIntegerForBitMask::BigIntegerForBitMask(const BigIntegerForBitMask &copy) {
    mpz_init(integ);
    mpz_set(integ, copy.integ);
    next = copy.next;
}

BigIntegerForBitMask &BigIntegerForBitMask::operator=(const BigIntegerForBitMask &copy) {
    //mpz_init(integ);
    mpz_set(integ, copy.integ);
    next = copy.next;
    return *this;
}

BigIntegerForBitMask BigIntegerForBitMask::operator+(unsigned int i) {
    __mpz_struct * toRet = nullptr;
    mpz_init(toRet);
    mpz_add_ui(toRet, integ, i);
    return BigIntegerForBitMask{toRet, 0};
}


BigIntegerForBitMask BigIntegerForBitMask::operator-(unsigned int i) {
    __mpz_struct * toRet = nullptr;
    mpz_init(toRet);
    mpz_sub_ui(toRet, integ, i);
    return BigIntegerForBitMask{toRet, 0};
}


BigIntegerForBitMask::BigIntegerForBitMask(size_t size, void *ptr) {
    mpz_init(integ);
    mpz_import (integ, size, 1, 1,1,0, ptr);
    next = 0;
}

BigIntegerForBitMask::BigIntegerForBitMask(struct iovec *ptr) : BigIntegerForBitMask{ptr->iov_len, ptr->iov_base} {
}

size_t BigIntegerForBitMask::serializedSize() {
    size_t size;
    mpz_export (nullptr, &size, 1,1,1,0, integ);
    return size;
}

bool BigIntegerForBitMask::copy(struct iovec &out) {
    size_t size;
    mpz_export (nullptr, &size, 1,1,1,0, integ);
    if (size > out.iov_len) {
        out.iov_base = realloc(out.iov_base, size);
    }
    if ((!out.iov_base)||(!size)) {
        return false;
    }
    out.iov_len = size;
    mpz_export(out.iov_base, &out.iov_len, 1,1,1,0, integ);
    return true;
}

size_t BigIntegerForBitMask::getMostSignificativeBit() const {
    if (mpz_cmp_ui(integ, 0) <= 0) return 0;
    return 1UL + mpz_sizeinbase (integ, 2);
}

size_t BigIntegerForBitMask::ll() const {
    if (mpz_cmp_ui(integ, 2) < 0)
        return 0;
    else
        return l<size_t>(getMostSignificativeBit()-1);
}

bool BigIntegerForBitMask::operator==(const BigIntegerForBitMask &rhs) const {
    return mpz_cmp(integ, rhs.integ) == 0;
}

BigIntegerForBitMask BigIntegerForBitMask::operator+(const BigIntegerForBitMask &i) const {
    __mpz_struct * toRet = nullptr;
    mpz_init(toRet);
    mpz_add(toRet, integ, i.integ);
    return BigIntegerForBitMask{toRet, 0};
}
