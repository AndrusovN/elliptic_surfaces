#pragma once
#define MULTIVARIABLE_POLYNOMIAL_HEADER
#include <map>
#include "field_concept.h"
#include "multivariable_monomial.h"

template <int variables_count, typename coefficient_t = int>
class MultivariablePolynomial {
public:
    MultivariablePolynomial();
    MultivariablePolynomial(const MultivariableMonomial<variables_count>& source);
    MultivariablePolynomial(coefficient_t C);

    template<Field Field_t>
    Field_t operator() (const std::array<Field_t, variables_count>& arguments) const;

    MultivariablePolynomial operator*(const MultivariablePolynomial& other) const;
    MultivariablePolynomial& operator*=(const MultivariablePolynomial& other);

    MultivariablePolynomial operator+(const MultivariablePolynomial& other) const;
    MultivariablePolynomial& operator+=(const MultivariablePolynomial& other);

    MultivariablePolynomial operator-(const MultivariablePolynomial& other) const;
    MultivariablePolynomial& operator-=(const MultivariablePolynomial& other);

    MultivariablePolynomial operator-() const;

    std::string to_string() const;

    template <int vars, typename coefs>
    friend std::ostream& operator<<(std::ostream&, const MultivariablePolynomial<vars, coefs>&);

    bool operator==(const MultivariablePolynomial<variables_count, coefficient_t>& other) const;
private:
    MultivariablePolynomial(std::map<MultivariableMonomial<variables_count>, coefficient_t> coeffs);

    std::map<MultivariableMonomial<variables_count>, coefficient_t> coefficients;
};


#include "multivariable_polynomial.cpp"
