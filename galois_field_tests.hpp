#pragma once
#include <gtest/gtest.h>
#include <random>
#include "galois_field.h"

TEST(GaloisFieldTests, GetPrimeSecondDegree) {
    auto poly = get_prime_polynomial<5>(2);
    for (int i = 0; i < 5; ++i) {
        ASSERT_NE(poly(i), 0);
    }
}

TEST(GaloisFieldTests, GetPrimeThirdDegree) {
    auto poly = get_prime_polynomial<5>(3);
    for (int i = 0; i < 5; ++i) {
        ASSERT_NE(poly(i), 0);
    }
}

TEST(GaloisFieldTests, GetPrimeSevenSecondDegree) {
    auto poly = get_prime_polynomial<7>(2);
    for (int i = 0; i < 7; ++i) {
        ASSERT_NE(poly(i), 0);
    }
}

TEST(GaloisFieldTests, GetPrimeTwentyThreeSecondDegree) {
    auto poly = get_prime_polynomial<23>(2);
    for (int i = 0; i < 23; ++i) {
        ASSERT_NE(poly(i), 0);
    }
}

TEST(GaloisFieldTests, DefaultConstructor) {
    GaloisFieldElement<5, 2> v;
    ASSERT_NE((GaloisFieldElement<5, 2>::get_mod()), 0);
    ASSERT_EQ(v, (GaloisFieldElement<5, 2>(PrimeFieldElement<5>(0))));
}

TEST(GaloisFieldTests, ValueConstructor) {
    GaloisFieldElement<5, 2> v(Polynomial<PrimeFieldElement<5>>({1,1}));
    ASSERT_NE((GaloisFieldElement<5, 2>::get_mod()), 0);
    ASSERT_EQ(v, (GaloisFieldElement<5, 2>(1)
            + GaloisFieldElement<5, 2>(Polynomial<PrimeFieldElement<5>>({0, 1}))));
}

std::mt19937 galois_field_random;
GaloisFieldElement<5, 2> random_element() {
    PrimeFieldElement<5> k, b;
    do {
        k = galois_field_random();
    } while (k == 0);
    if (galois_field_random() % 2) {
        return GaloisFieldElement<5, 2>(Polynomial<PrimeFieldElement<5>>(k));
    }

    do {
        b = galois_field_random();
    } while (b == 0);
    return GaloisFieldElement<5, 2>(Polynomial<PrimeFieldElement<5>>({b, k}));
}

TEST(GaloisFieldTests, Inverse) {
    for (int i = 0; i < 1000; ++i) {
        auto rand_val = random_element();
        if (rand_val == GaloisFieldElement<5, 2>(0)) continue;
        ASSERT_EQ(rand_val * rand_val.inverse(), (GaloisFieldElement<5, 2>(1)));
    }
}

TEST(GaloisFieldTests, UnariMinus) {
    for (int i = 0; i < 1000; ++i) {
        auto rand_val = random_element();
        ASSERT_EQ(rand_val + -rand_val, (GaloisFieldElement<5, 2>(0)));
    }
}
