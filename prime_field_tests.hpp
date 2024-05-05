#pragma once
#include <gtest/gtest.h>
#include "prime_field.h"

TEST(PrimeFieldTests, ConstructorBelowZero) {
    PrimeFieldElement<7> a = -179;
    ASSERT_EQ(a, 3);
}

TEST(PrimeFieldTests, ConstructorBig) {
    PrimeFieldElement<7> a = 57;
    ASSERT_EQ(a, 1);
}

TEST(PrimeFieldTests, DefaultConstructor) {
    PrimeFieldElement<7> a;
    ASSERT_EQ(a, 0);
}

TEST(PrimeFieldTests, CopyConstructor) {
    PrimeFieldElement<7> a = 5;
    PrimeFieldElement<7> b = a;
    ASSERT_EQ(a, b);
}

TEST(PrimeFieldTests, CopyAssignment) {
    PrimeFieldElement<7> a = 4;
    PrimeFieldElement<7> b = 2;
    a = b;
    ASSERT_EQ(a, b);
    ASSERT_EQ(a, 2);
}

TEST(PrimeFieldTests, MoveConstructor) {
    PrimeFieldElement<7> a = 4;
    PrimeFieldElement<7> b = std::move(a);

    ASSERT_EQ(b, 4);
    for (int i = 0; i < 7; ++i) {
        ASSERT_NE(a, i);
    }
}

TEST(PrimeFieldTests, Inverse) {
    PrimeFieldElement<11> a = 4;
    ASSERT_EQ(a.inverse(), 3);
}

TEST(PrimeFieldTests, InverseThrow) {
    PrimeFieldElement<11> a = 11;
    ASSERT_THROW(a.inverse(), MathException);
}

TEST(PrimeFieldTests, Power) {
    PrimeFieldElement<11> a = 2;
    ASSERT_EQ(a.pow(7), 7);
    ASSERT_EQ(a.pow(5), 10);
    ASSERT_EQ(a.pow(2), 4);
    ASSERT_EQ(a.pow(0), 1);
    ASSERT_EQ(a.pow(1), 2);
}

TEST(PrimeFieldTests, Sum) {
    PrimeFieldElement<7> a = 3;
    PrimeFieldElement<7> b = 2;
    ASSERT_EQ(a + b, 5);
}

TEST(PrimeFieldTests, SumOver) {
    PrimeFieldElement<7> a = 6;
    PrimeFieldElement<7> b = 5;
    ASSERT_EQ(a + b, 4);
}

TEST(PrimeFieldTests, Diff) {
    PrimeFieldElement<7> a = 6;
    PrimeFieldElement<7> b = 5;
    ASSERT_EQ(a - b, 1);
}


TEST(PrimeFieldTests, DiffBelowZero) {
    PrimeFieldElement<11> a = 6;
    PrimeFieldElement<11> b = 5;
    ASSERT_EQ(b - a, 10);
}


TEST(PrimeFieldTests, Product) {
    PrimeFieldElement<7> a = 6;
    PrimeFieldElement<7> b = 5;
    ASSERT_EQ(a * b, 2);
}

TEST(PrimeFieldTests, Division) {
    PrimeFieldElement<7> a = 6;
    PrimeFieldElement<7> b = 5;
    ASSERT_EQ(a / b, 4);
}


TEST(PrimeFieldTests, ToInt) {
    PrimeFieldElement<7> a = 6;
    ASSERT_EQ(a.to_int(), 6);
}

TEST(PrimeFieldTests, ToIntOver) {
    PrimeFieldElement<7> a = 67;
    ASSERT_EQ(a.to_int(), 4);
}
