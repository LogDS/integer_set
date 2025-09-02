#include <iostream>
#include "basic_operations.h"
// #include "mpzDAG.h"

// template <> struct tricotomia<BigIntegerForBitMask> {
//     BigIntegerForBitMask g;
//     BigIntegerForBitMask p;
//     BigIntegerForBitMask d;
//     char is1Or2;
//     char is1Either2;
//
//     tricotomia(BigIntegerForBitMask g, BigIntegerForBitMask p, BigIntegerForBitMask d) : g(g), p(p), d(d), is1Or2{0}, is1Either2{0}
//     {
//     }
//
//
//
//
// };

#include "BIgInteger2.h"

int main() {

    dag<size_t> dg(0, 1);
    {
        auto two = dg.get(2);
        auto four = dg.get(4);
        auto six = dg.get(8);
        tricotomiaNode *ptr4 = dg.get(50);
        tricotomiaNode *ptr17 = dg.get(17);
        tricotomiaNode *ptr12 = dg.get(819);
        tricotomiaNode *ptra12 = dg.get(12);
        assert((int)dg.Or(two, four)->compareWith(six) < 0);
        assert(dg.And(ptr4, ptr17)->compute<unsigned long>() == 16); // 50 & 17 == 16
        assert(dg.And(four, ptr17)->compute<unsigned long>() ==0);
        assert(dg.And(ptr4, ptr12)->compute<unsigned long>() == 50);
        assert(dg.And(ptr4, ptra12)->compute<unsigned long>() == 0);
        assert(dg.And(four, ptra12)->compute<unsigned long>() == 4);
        assert(dg.And(ptr12, ptr17)->compute<unsigned long>() == 17);
        assert(dg.And(ptra12, ptr17)->compute<unsigned long>() == 0);
        assert(dg.Or(ptr4, ptr17)->compute<unsigned long>() == 51);
        assert(dg.Or(four, ptr17)->compute<unsigned long>() == 21);
        assert(dg.Or(ptr4, ptr12)->compute<unsigned long>() == 819);
        assert(dg.Or(four, ptra12)->compute<unsigned long>() == 12);
        assert(dg.Or(ptr12, ptr17)->compute<unsigned long>() == 819);
        assert(dg.Or(ptra12, ptr17)->compute<unsigned long>() == 29);

    }

    BigInteger2 zero(0), one(1);
    dag<BigInteger2> dag2(zero, one);
    {
        auto two = dag2.get(2);
        auto four = dag2.get(4);
        auto six = dag2.get(8);
        tricotomiaNode *ptr4 = dag2.get(50);
        tricotomiaNode *ptr17 = dag2.get(17);
        tricotomiaNode *ptr12 = dag2.get(819);
        tricotomiaNode *ptra12 = dag2.get(12);
        assert((int)dag2.Or(two, four)->compareWith(six) < 0);
        assert(dag2.And(ptr4, ptr17)->compute<unsigned long>() == 16); // 50 & 17 == 16
        assert(dag2.And(four, ptr17)->compute<unsigned long>() ==0);
        assert(dag2.And(ptr4, ptr12)->compute<unsigned long>() == 50);
        assert(dag2.And(ptr4, ptra12)->compute<unsigned long>() == 0);
        assert(dag2.And(four, ptra12)->compute<unsigned long>() == 4);
        assert(dag2.And(ptr12, ptr17)->compute<unsigned long>() == 17);
        assert(dag2.And(ptra12, ptr17)->compute<unsigned long>() == 0);
        assert(dag2.Or(ptr4, ptr17)->compute<unsigned long>() == 51);
        assert(dag2.Or(four, ptr17)->compute<unsigned long>() == 21);
        assert(dag2.Or(ptr4, ptr12)->compute<unsigned long>() == 819);
        assert(dag2.Or(four, ptra12)->compute<unsigned long>() == 12);
        assert(dag2.Or(ptr12, ptr17)->compute<unsigned long>() == 819);
        assert(dag2.Or(ptra12, ptr17)->compute<unsigned long>() == 29);
    }


///*
//    tricotomia<unsigned long> t{818};
//    std::cout << t << std::endl;
//    // 818 = 50+2^(2^3)*3
//    // Quindi, mi chiedo come rappresentare 50 e 3
//
//    tricotomia<unsigned long> ter{3};
//    std::cout << ter << std::endl;
//    // 3 = 1+2*1
//    // Ottengo i numeri 1 e 2, mi fermo
//
//    tricotomia<unsigned long> l{50};
//    std::cout << l << std::endl;
//    // 50 = 2+2^(2^2)*3
//    // Ottengo i numeri 2 e 3, di cui 3 è già stato calcolato*/
//
//    dag<unsigned long> hashmap;
//
//    /* Printing exponents
//     *
//     * unsigned long long X = 1;
//    for (int i = 0; i<35; i++) {
//        std::cout << "for i = " << i << " we have: ";
//        hashmap.get(X)->print<unsigned long long>();
//        X = X << 1;
//    }*/
//
//
//    // TODO: printing 2 hashmap.get(2)->print<unsigned long>();
//
//
//    tricotomiaNode *ptr4 = hashmap.get(50);
//    tricotomiaNode *ptr17 = hashmap.get(17);
//    tricotomiaNode *ptr12 = hashmap.get(819);
//
////    std::cout << (int)ptr4->compareWith<long>(ptr12) << std::endl;
//  //  std::cout << (int)ptr4->compareWith<long>(ptr17) << std::endl;
//
//
//    /*std::cout << hashmap.And(ptr4, ptr17)->compute<unsigned long>() << std::endl; // 50 & 17 == 16
//    std::cout << hashmap.And(ptr4, ptr12)->compute<unsigned long>() << std::endl; // == 50
//    std::cout << hashmap.And(ptr12, ptr17)->compute<unsigned long>() << std::endl; // == 17
//*/
//
//    std::cout << hashmap.Or(ptr4, ptr17)->compute<unsigned long>() << std::endl; // 50 | 17 = 51
//    std::cout << hashmap.Or(ptr4, ptr12)->compute<unsigned long>() << std::endl; // == 50
//    std::cout << hashmap.Or(ptr12, ptr17)->compute<unsigned long>() << std::endl; // == 17
//    /* hashmap.get(3)->print<unsigned long>();
//
//     tricotomia<unsigned long> l{1,1,0};
//     std::cout << l << std::endl;*/

    return 0;
}