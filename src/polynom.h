#pragma once
#define POLYNOMIAL_HEADER
#include <vector>
#include <complex>
#include <string>
#include "field_concept.h"
using c_field = std::complex<long double>;

size_t revert_binary(size_t, size_t);
void reorder_for_fft(std::vector<c_field>&);
void fft(std::vector<c_field>&, bool);

template <Field Field_t>
class Polynomial {
public:
    Polynomial();
    Polynomial(Field_t element);
    Polynomial(const Polynomial<Field_t>& source);
    Polynomial(Polynomial<Field_t>&& source) noexcept;
    Polynomial(int element);
    explicit Polynomial(const std::vector<Field_t>& data);
    Polynomial<Field_t>& operator=(const Polynomial<Field_t>& other);

    Polynomial<Field_t> operator+(const Polynomial& other) const;
    Polynomial<Field_t> operator-(const Polynomial& other) const;
    Polynomial<Field_t> operator*(const Polynomial& other) const;
    Polynomial<Field_t> operator%(const Polynomial& other) const;
    Polynomial<Field_t> operator/(const Polynomial& other) const;

    Polynomial<Field_t> operator-() const;
    [[nodiscard]] int degree() const;
    Polynomial<Field_t> power(int n) const;

    Polynomial<Field_t>& operator+=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator-=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator*=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator%=(const Polynomial<Field_t>& other);
    Polynomial<Field_t>& operator/=(const Polynomial<Field_t>& other);

    std::pair<Polynomial<Field_t>, Polynomial<Field_t>> divide_with_remainder(const Polynomial<Field_t>& other) const;

    bool operator==(const Polynomial<Field_t>& other) const;

    Field_t operator() (const Field_t& value) const;

    [[nodiscard]] std::string to_string() const;
    std::string to_string_unified() const;

    static Polynomial<Field_t> from_string_unified(const std::string& source);

    Polynomial<Field_t> multiply_slow(const Polynomial<Field_t>& other);
    Polynomial<Field_t> multiplied_slow_by(const Polynomial<Field_t>& other);
    std::pair<Polynomial, Polynomial> divide_slow(const Polynomial& other) const;

    Field_t get_coefficient(size_t degree) const;

    Polynomial<Field_t> next() const;
private:
    [[nodiscard]] std::vector<c_field> to_complex(size_t target_size) const;
    void build_from_complex(const std::vector<c_field>& coefs);
    Polynomial<Field_t> inverse_polynomial() const;
    void remove_leading_zeroes();
    Polynomial<Field_t> reversed() const;

    Polynomial<Field_t>& multiply(const Polynomial<Field_t>& other);
    Polynomial<Field_t> multiplied_by(const Polynomial<Field_t>& other);

    std::vector<Field_t> coefficients;
};

template <Field Field_t>
std::ostream& operator<<(std::ostream& out, const Polynomial<Field_t>& poly);

template <Field Field_t>
Polynomial<Field_t> gcd(const Polynomial<Field_t>&, const Polynomial<Field_t>&);

template <Field Field_t>
Polynomial<Field_t> get_divisor(const Polynomial<Field_t>& source);

#include "polynom.cpp"
