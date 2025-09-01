//
// Created by gyankos on 01/09/25.
//

#ifndef DECOMPOSE_BIGINTEGER2_H
#define DECOMPOSE_BIGINTEGER2_H

#include <cstdint>
#include <gmp.h>
#include <string>

#include "basic_operations.h"
#include "../submodules/murmur3/MurmurHash3.h"

static inline size_t MurmurHash3_size_t(const void *key, int len, uint32_t seed) {
    // https://stackoverflow.com/q/62452712/1376095
#if SIZE_MAX==0xffffffff
    size_t result;
    MurmurHash3_x86_32(key, len, seed, &result);
    return result;
#elif SIZE_MAX==0xffffffffffffffff
    size_t result[2];
    MurmurHash3_x64_128(key, len, seed, &result);
    return result[0] ^ result[1];
#else
#error cannot determine correct version of MurmurHash3, because SIZE_MAX is neither 0xffffffff nor 0xffffffffffffffff
#endif
}

struct BigInteger2 {
    mpz_t integ;

public:
    /**
     * Initializing the BigInteger from a string
     * @param val Base 10 String representation of the string
     */
    BigInteger2(const std::string& val, size_t base = 10) {
        mpz_init_set_str(integ, val.c_str(), base);
    }

    /**
     * Initializing the number from an integer value
     * @param ui Unsigned integer value
     */
    BigInteger2(unsigned long long ui = 0) {
        mpz_init_set_ui(integ, ui);
    }

    BigInteger2& operator=(unsigned long long ui) {
        mpz_set_ui(integ, ui);
        return *this;
    }

    /**
     * Copy assignment constructor
     * @param other
     */
    BigInteger2(const BigInteger2 &other) {
        mpz_init_set(integ, other.integ);
    }

    void add_bit(size_t idx) {
        mpz_setbit(integ, idx);
    }
    void del_bit(size_t idx) {
        mpz_clrbit(integ, idx);
    }
    void complement_bit(size_t idx) {
        mpz_combit(integ, idx);
    }
    bool has_bit(size_t idx) const {
        return mpz_tstbit(integ, idx);
    }

    /**
     * Assignment operator
     * @param other Other integer from which copy the element
     * @return
     */
    BigInteger2& operator=(const BigInteger2 &other) {
        mpz_set(integ, other.integ);
        return *this;
    }

    virtual ~BigInteger2() { mpz_clear(integ); }

    inline size_t hashCode() const {
        // found 1846872219 by randomly typing digits on my keyboard
        // https://stackoverflow.com/a/62453595/1376095
        return MurmurHash3_size_t(integ->_mp_d, integ->_mp_size * sizeof(mp_limb_t),
                1846872219);
    }
    BigInteger2 operator+(const BigInteger2 &other) const {
        BigInteger2 result;
        mpz_add(result.integ, integ, other.integ);
        return result;
    }
    BigInteger2 operator+(size_t other) const {
        BigInteger2 result;
        mpz_add_ui(result.integ, integ, other);
        return result;
    }

    BigInteger2 operator*(const BigInteger2 &other) const {
        BigInteger2 result;
        mpz_mul(result.integ, integ, other.integ);
        return result;
    }
    BigInteger2 operator*( size_t other) const {
        BigInteger2 result;
        mpz_mul_ui(result.integ, integ, other);
        return result;
    }

    BigInteger2& operator+=(const BigInteger2 &other)  {
        mpz_t tmp;
        mpz_init(tmp);
        mpz_add(tmp, integ, other.integ);
        mpz_swap(integ, tmp);
        mpz_clear(tmp);
        return *this;
    }
    BigInteger2& operator+=(size_t other)  {
        mpz_t tmp;
        mpz_init(tmp);
        mpz_add_ui(tmp, integ, other);
        mpz_swap(integ, tmp);
        mpz_clear(tmp);
        return *this;
    }
    BigInteger2& operator*=(unsigned long long other)  {
        mpz_t tmp;
        mpz_init(tmp);
        mpz_mul_ui(tmp, integ, other);
        mpz_swap(integ, tmp);
        mpz_clear(tmp);
        return *this;
    }
    BigInteger2& operator*=(const BigInteger2 &other)  {
        mpz_t tmp;
        mpz_init(tmp);
        mpz_mul(tmp, integ, other.integ);
        mpz_swap(integ, tmp);
        mpz_clear(tmp);
        return *this;
    }
    inline size_t getMostSignificativeBit() const {
        if (mpz_cmp_ui(integ, 0) <= 0) return 0;
        return 1UL + mpz_sizeinbase (integ, 2);
    }
    bool operator<(const BigInteger2 &other) const {
        return mpz_cmp(integ, other.integ) < 0;
    }
    bool operator<(size_t other) const {
        return  mpz_cmp_ui (integ, other) < 0;
    }
    bool operator>(const BigInteger2 &other) const {
        return mpz_cmp(integ, other.integ) > 0;
    }
    bool operator>(size_t other) const {
        return  mpz_cmp_ui (integ, other) > 0;
    }
    bool operator<=(const BigInteger2 &other) const {
        return mpz_cmp(integ, other.integ) <= 0;
    }
    bool operator<=(size_t other) const {
        return  mpz_cmp_ui (integ, other) <= 0;
    }
    bool operator>=(const BigInteger2 &other) const {
        return mpz_cmp(integ, other.integ) >= 0;
    }
    bool operator>=(size_t other) const {
        return  mpz_cmp_ui (integ, other) >= 0;
    }
    bool operator==(const BigInteger2 &other) const {
        return mpz_cmp(integ, other.integ) == 0;
    }
    BigInteger2& operator/=(const BigInteger2 &other) const {
        BigInteger2 result(*this);

    }

    operator size_t() const {
        return mpz_get_ui(integ);
    }

    int sgn() const {
        return mpz_sgn(integ);
    }

    size_t ll() const {
        if (mpz_cmp_ui(integ, 2) < 0)
            return 0;
        else
            return ::l<size_t>(this->l()-1);
    }

    BigInteger2 expexp2() const {
        BigInteger2 result;
        mpz_ui_pow_ui(result.integ, 2, std::pow(2, mpz_get_ui(integ)));
        return result;
    }

    static BigInteger2 expexp2(size_t i) {
        BigInteger2 result;
        mpz_ui_pow_ui(result.integ, 2, std::pow(2, i));
        return result;
    }

    size_t l() const {
        if (*this <= 0)
            return 0;
        else {
            auto tmp = getMostSignificativeBit()-1;
            return tmp; //
        }
    }

    friend std::ostream& operator<<(std::ostream &out, const BigInteger2& self) {
        auto ptr = mpz_get_str(nullptr, 10, self.integ);
        out << ptr;
        free(ptr);
        return out;
    }
};



namespace std {
    template <> struct hash<BigInteger2> {
        size_t operator()(const BigInteger2 &x) const {
            return x.hashCode();
        }
    };
}


/**
 * This class only performs the computation for initializing the number as expected.
 * @tparam Numeric
 */
template <> struct tricotomia<BigInteger2> {
    BigInteger2 g;
    size_t p;
    BigInteger2 d;
    char is1Or2;
    char is1Either2;

    tricotomia(const BigInteger2& g, size_t p, const BigInteger2& d) : g(g), p(p), d(d), is1Or2{0}, is1Either2{0}
    {
    }

    tricotomia(const BigInteger2& n) {
        if (n <= 1) {
            is1Or2 = true;
            is1Either2 = n <= 1;
        } else {
            is1Or2 = false;
            auto p_int = n.ll() - 1;
            p = p_int;
            auto tmp = BigInteger2::expexp2(p_int);
            mpz_fdiv_qr(g.integ, d.integ, n.integ, tmp.integ);
            // g = n / tmp;
            // d = n % tmp;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const tricotomia &tricotomia) {
        os << "molto: " << tricotomia.g << " p: " << tricotomia.p << " summato: " << tricotomia.d;
        auto test = BigInteger2::expexp2(tricotomia.p);
        test *= tricotomia.g;
        test += tricotomia.d;
        os << " test: " << test;
        return os;
    }
};

#endif //DECOMPOSE_BIGINTEGER2_H