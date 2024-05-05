#pragma once
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "multivariable_monomial.h"
#include "multivariable_polynomial.h"

TEST(MultivariableTests, MonomialConstructor) {
    MultivariableMonomial<3> m;
    ASSERT_EQ(m.to_string(), "x_{1}^{0}x_{2}^{0}x_{3}^{0}");
}

TEST(MultivariableTests, MonomialConstructorFromPowers) {
    std::array<int, 3> data{1,2,3};
    MultivariableMonomial<3> m(data);
    ASSERT_EQ(m.to_string(), "x_{1}^{1}x_{2}^{2}x_{3}^{3}");
}

TEST(MultivariableTests, MonomialProduct) {
    std::array<int, 3> data1{1,2,0};
    MultivariableMonomial<3> m1(data1);

    std::array<int, 3> data2{2,3,4};
    MultivariableMonomial<3> m2(data2);
    ASSERT_EQ((m1 * m2).to_string(), "x_{1}^{3}x_{2}^{5}x_{3}^{4}");
}

TEST(MultivariableTests, MonomialApply) {
    std::array<int, 3> data{1,2,2};
    MultivariableMonomial<3> m(data);

    std::array<PrimeFieldElement<7>, 3> args{1,2,3};
    ASSERT_EQ(m(args), 1);

    std::array<PrimeFieldElement<7>, 3> another_args{1,5,2};
    ASSERT_EQ(m(another_args), 2);
}

TEST(MultivariableTests, MonomialXConstructor) {
    MultivariableMonomial<3> m = MultivariableMonomial<3>::X(0) * MultivariableMonomial<3>::X(2);
    ASSERT_EQ(m.to_string(), "x_{1}^{1}x_{2}^{0}x_{3}^{1}");
}

TEST(MultivariableTests, MonomialComparison) {
    MultivariableMonomial<3> m({1,2,3});
    MultivariableMonomial<3> p({1,1,4});
    ASSERT_LE(p, m);
}

TEST(MultivariableTests, MonomialComparisonAnother) {
    MultivariableMonomial<3> m({1,2,3});
    MultivariableMonomial<3> p({1,2,4});
    ASSERT_GE(p, m);
}

TEST(MultivariableTests, MonomialComparisonEq) {
    MultivariableMonomial<3> m({1,2,3});
    MultivariableMonomial<3> p({1,2,3});
    ASSERT_EQ(m, p);
}

TEST(MultivariableTests, PolynomialDefaultConstructor) {
    ASSERT_EQ((MultivariablePolynomial<3, int>().to_string()), "0");
}

TEST(MultivariableTests, PolynomialConstructorFromMonomial) {
    MultivariableMonomial<3> m({1,2,3});
    MultivariablePolynomial<3, int> p(m);

    ASSERT_EQ(p.to_string(), "1" + m.to_string() + "+0");
}

TEST(MultivariableTests, PolynomialSum) {
    MultivariableMonomial<3> m1({1,2,3});
    MultivariablePolynomial<3, int> p1(m1);

    MultivariableMonomial<3> m2({2,3,1});
    MultivariablePolynomial<3, int> p2(m2);
    std::string s1 = "1" + m1.to_string() + "+1" + m2.to_string() + "+0";

    ASSERT_EQ((p1 + p2).to_string(), s1);
}

TEST(MultivariableTests, PolynomialDiff) {
    MultivariableMonomial<3> m1({1,2,3});
    MultivariablePolynomial<3, int> p1(m1);

    MultivariableMonomial<3> m2({2,3,1});
    MultivariablePolynomial<3, int> p2(m2);
    std::string s1 = "1" + m1.to_string() + "+-1" + m2.to_string() + "+0";

    ASSERT_EQ((p1 - p2).to_string(), s1);
}

TEST(MultivariableTests, PolynomialSumSame) {
    MultivariableMonomial<3> m1({1,2,3});
    MultivariablePolynomial<3, int> p1(m1);

    MultivariableMonomial<3> m2({1,2,3});
    MultivariablePolynomial<3, int> p2(m2);
    std::string s1 = "2" + m1.to_string() + "+0";

    ASSERT_EQ((p1 + p2).to_string(), s1);
}

TEST(MultivariableTests, PolynomialProduct) {
    MultivariablePolynomial<3, int> p1 = MultivariablePolynomial<3, int>(MultivariableMonomial<3>({1,1,1}))
        + MultivariableMonomial<3>({1,2,3});

    MultivariablePolynomial<3, int> p2 = MultivariablePolynomial<3, int>(MultivariableMonomial<3>({2,2,2}))
                         + MultivariableMonomial<3>({2,4,6}) + MultivariableMonomial<3>({2,3,4}) + MultivariableMonomial<3>({2,3,4});

    ASSERT_EQ(p1 * p1, p2);
}

TEST(MultivariableTests, PolynomialTimesConst) {
    MultivariableMonomial<3> m1({1,2,3});
    MultivariablePolynomial<3, int> p1 = MultivariablePolynomial<3, int>(m1) + m1 + m1;
    MultivariablePolynomial<3, int> p2 = m1;
    p2 *= 3;
    ASSERT_EQ(p1, p2);
}

TEST(MultivariableTests, PolynomialApply) {
    MultivariableMonomial<3> m1({1,2,3});
    MultivariableMonomial<3> m2({1,1,1});

    MultivariablePolynomial<3, int> p1 = MultivariablePolynomial<3, int>(m1) * 2 + m2;
    std::array<PrimeFieldElement<7>, 3> args = {2,3,4};
    ASSERT_EQ(p1(args), 4);
}
