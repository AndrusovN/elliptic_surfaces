#pragma once
#include <gtest/gtest.h>
#include <random>
#include "polynom.h"
#include "prime_field.h"

using F = PrimeFieldElement<11>;

TEST(PolynomialTests, DefaultConstructor) {
    Polynomial<F> p;
    ASSERT_EQ(p, 0);
}

TEST(PolynomialTests, ConstructFromConstant) {
    Polynomial<F> p = 7;
    ASSERT_EQ(p, 7);
}

TEST(PolynomialTests, ConstructFromFieldElement) {
    Polynomial<F> p = F(5);
    ASSERT_EQ(p, 5);
}

TEST(PolynomialTests, ConstructorFromVector) {
    Polynomial<F> p({1,2,3,4});
    ASSERT_EQ(p(0), 1);
    ASSERT_EQ(p(1), 10);
    ASSERT_EQ(p(-1), -2);
    ASSERT_EQ(p(2), 49);
}

TEST(PolynomialTests, Add) {
    Polynomial<F> p({1,2,3});
    Polynomial<F> q({1,0,4,2});
    ASSERT_EQ(p + q, Polynomial<F>({2,2,7,2}));
}

TEST(PolynomialTests, Subtract) {
    Polynomial<F> p({1,2,3});
    Polynomial<F> q({1,0,4,2});
    ASSERT_EQ(p - q, Polynomial<F>({0,2,10,9}));
}

TEST(PolynomialTests, Mult) {
    Polynomial<F> p({1,2});
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(p * q, Polynomial<F>({1, 2, 4, 8}));
}

TEST(PolynomialTests, Remainder) {
    Polynomial<F> p({1,2});
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(q % p, 2);
}

TEST(PolynomialTests, UnaryMinus) {
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(q + -q, 0);
}

TEST(PolynomialTests, Deg) {
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(q.degree(), 2);
}

TEST(PolynomialTests, Power) {
    Polynomial<F> q({1,1});
    ASSERT_EQ(q.power(4), Polynomial<F>({1,4,6,4,1}));
}

TEST(PolynomialTests, Apply) {
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(q(0), 1);
    ASSERT_EQ(q(1), 5);
    ASSERT_EQ(q(2), 17);
    ASSERT_EQ(q(3), 37);
    ASSERT_EQ(q(4), 65);
    ASSERT_EQ(q(5), 101);
    ASSERT_EQ(q(6), 145);
    ASSERT_EQ(q(7), 197);
    ASSERT_EQ(q(8), 257);
    ASSERT_EQ(q(9), 325);
    ASSERT_EQ(q(10), 401);
}

TEST(PolynomialTests, ToString) {
    Polynomial<F> q({1, 3, 1, 4, 5});
    ASSERT_EQ(q.to_string(), "5x^{4} + 4x^{3} + x^{2} + 3x + 1");
}

std::mt19937 my_random(1791791791);

Polynomial<F> random_polynomial(int degree) {
    std::vector<F> coefficients(degree + 1, 0);
    for (int i = 0; i < degree; ++i) {
        coefficients[i] = my_random();
    }
    do {
        coefficients.back() = my_random();
    } while (coefficients.back() == 0);
    return Polynomial<F>(coefficients);
}

TEST(PolynomialTests, MultSlow) {
    Polynomial<F> p({1,2});
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(p.multiplied_slow_by(q), Polynomial<F>({1, 2, 4, 8}));
}

TEST(PolynomialTests, RemainderSlow) {
    Polynomial<F> p({1,2});
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(q.divide_slow(p).second, 2);
}

TEST(PolynomialTests, DivideSlow) {
    Polynomial<F> p({1,2});
    Polynomial<F> q({1,0,4});
    ASSERT_EQ(q.divide_slow(p).first, Polynomial<F>({-1, 2}));
}

TEST(PolynomialTests, MultiplyFastRandomTests) {
    for (int i = 0; i < 1000; ++i) {
        auto p = random_polynomial(57);
        auto q = random_polynomial(75);
        ASSERT_EQ(p * q, p.multiplied_slow_by(q));
    }
}

TEST(PolynomialTests, MultiplyFastRandomTestsHeavy) {
    for (int i = 0; i < 20; ++i) {
        auto p = random_polynomial(1234);
        auto q = random_polynomial(1234);
        ASSERT_EQ(p * q, p.multiplied_slow_by(q));
    }
}

TEST(PolynomialTests, DivideFastRandomTestsSmall) {
    for (int i = 0; i < 1000; ++i) {
        auto p = random_polynomial(7);
        auto q = random_polynomial(5);

        ASSERT_EQ(p.divide_with_remainder(q), p.divide_slow(q)) << p << "; " << q;
    }
}

TEST(PolynomialTests, DivideFastRandomTestsDegreeOne) {
    for (int i = 0; i < 1000; ++i) {
        auto p = random_polynomial(3);
        auto q = random_polynomial(1);

        ASSERT_EQ(p.divide_with_remainder(q), p.divide_slow(q)) << p << "; " << q;
    }
}

TEST(PolynomialTests, DivideFastRandomTestsDegreeTwo) {
    for (int i = 0; i < 1000; ++i) {
        auto p = random_polynomial(7);
        auto q = random_polynomial(2);

        ASSERT_EQ(p.divide_with_remainder(q), p.divide_slow(q)) << p << "; " << q;
    }
}

TEST(PolynomialTests, DivideFastRandomTests) {
    for (int i = 0; i < 1000; ++i) {
        auto p = random_polynomial(75);
        auto q = random_polynomial(57);
        ASSERT_EQ(p.divide_with_remainder(q), p.divide_slow(q));
    }
}

TEST(PolynomialTests, DivideFastRandomTestsHeavy) {
    for (int i = 0; i < 20; ++i) {
        auto p = random_polynomial(1234);
        auto q = random_polynomial(1134);
        ASSERT_EQ(p.divide_with_remainder(q), p.divide_slow(q));
    }
}

bool polynomials_are_equivalent(Polynomial<F> a, Polynomial<F> b) {
    auto coef = a.get_coefficient(a.degree()) / b.get_coefficient(b.degree());
    return a == b * coef;
}

TEST(PolynomialTests, GCD) {
    Polynomial<F> p({1,1});
    Polynomial<F> q({2,1});
    auto A = p.power(7) * q.power(5);
    auto B = p.power(5) * q.power(7);
    auto C = gcd(A, B);
    auto D = p.power(5)*q.power(5);
    ASSERT_TRUE(polynomials_are_equivalent(C, D)) << A << "\n" << B << "\n" << C << "\n" << D << "\n";
}

TEST(PolynomialTests, GCDRandom) {
    for (int i = 0; i < 1000; ++i) {
        auto p = random_polynomial(17);
        auto q = random_polynomial(17);
        auto c = gcd(p, q);
        auto d = gcd_slow(p, q);
        ASSERT_EQ(p.divide_slow(c).second, 0) << p << "\n" << q << "\n" << c << "\n" << d;
        ASSERT_EQ(q.divide_slow(c).second, 0) << p << "\n" << q << "\n" << c << "\n" << d;
        ASSERT_EQ(c, d) << p << "\n" << q << "\n" << c << "\n" << d;
    }
}

TEST(PolynomialTests, GCDRandomHard) {
    for (int i = 0; i < 20; ++i) {
        auto g = random_polynomial(57);
        auto p = g * random_polynomial(75);
        auto q = g * random_polynomial(75);
        auto c = gcd(p, q);
        auto d = gcd_slow(p, q);
        ASSERT_EQ(p.divide_slow(c).second, 0) << p << "\n" << q << "\n" << c << "\n" << d;
        ASSERT_EQ(q.divide_slow(c).second, 0) << p << "\n" << q << "\n" << c << "\n" << d;
        ASSERT_EQ(c, d) << p << "\n" << q << "\n" << c << "\n" << d;
    }
}

TEST(PolynomialTests, ToStringUnified) {
    Polynomial<F> p({1,2,3,4});
    ASSERT_EQ(p.to_string_unified(), "4x^{3}+3x^{2}+2x^{1}+1x^{0}");
}

TEST(PolynomialTests, FromStringUnified) {
    Polynomial<F> p({1,2,3,4});
    ASSERT_EQ(Polynomial<F>::from_string_unified("4x^{3}+3x^{2}+2x^{1}+1x^{0}"), p);
}

TEST(PolynomialTests, ToStringUnifiedRandom) {
    for (int i = 0; i < 1000; ++i) {
        auto poly = random_polynomial(1791);
        ASSERT_EQ(Polynomial<F>::from_string_unified(poly.to_string_unified()), poly);
    }
}
