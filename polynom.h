#pragma once
#define POLYNOM_HEADER
#include <vector>
#include <complex>
#include <string>
#include "field_concept.h"
using c_field = std::complex<long double>;

template <Field Field_t>
class Polynomial {
public:
    Polynomial();
    Polynomial(Field_t element);
    Polynomial(const Polynomial<Field_t>& source);
    Polynomial(Polynomial<Field_t>&& source) noexcept;
    explicit Polynomial(const std::vector<Field_t>& data);

    Polynomial<Field_t> operator+(const Polynomial& other) const;
    Polynomial<Field_t> operator-(const Polynomial& other) const;
    Polynomial<Field_t> operator*(const Polynomial& other) const;
    Polynomial<Field_t> operator%(const Polynomial& other) const;

    Polynomial<Field_t> operator-() const;
    int degree() const;
    Polynomial<Field_t> power(int n) const;

    Polynomial<Field_t>& operator+=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator-=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator*=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator%=(const Polynomial<Field_t>& other);

    std::string to_string() const;
private:
    std::vector<c_field> to_complex(size_t target_size) const;
    void build_from_complex(const std::vector<c_field>& coefs);
    Polynomial<Field_t> inverse_polynomial() const;
    void remove_leading_zeroes();
    Polynomial<Field_t> reversed() const;

    std::vector<Field_t> coefficients;
};

#include "polynom.cpp"