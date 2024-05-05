#pragma once
#define GALOIS_FIELD_HEADER
#include "polynom.h"

template <int p>
Polynomial<PrimeFieldElement<p>> get_prime_polynomial(int d);

template <int p>
std::vector<Polynomial<PrimeFieldElement<p>>> get_prime_cached_info();

template <int p>
Polynomial<PrimeFieldElement<p>> get_prime_polynomial_cached(int d);

template <int p>
void prepare_polynomials(int max_degree);

template <Field Field_t>
std::pair<Polynomial<Field_t>, Polynomial<Field_t>> extended_euclidean_algorithm(const Polynomial<Field_t>&, const Polynomial<Field_t>&);


template<int p, int d>
class GaloisFieldElement {
public:
    using number = Polynomial<PrimeFieldElement<p>>;

    GaloisFieldElement();
    GaloisFieldElement(number val);

    GaloisFieldElement<p, d> inverse() const;
    GaloisFieldElement<p, d> pow(int power) const;
    GaloisFieldElement<p, d> operator-() const;

    GaloisFieldElement<p, d> operator+(const GaloisFieldElement<p, d>&) const;
    GaloisFieldElement<p, d> operator-(const GaloisFieldElement<p, d>&) const;
    GaloisFieldElement<p, d> operator*(const GaloisFieldElement<p, d>&) const;
    GaloisFieldElement<p, d> operator/(const GaloisFieldElement<p, d>&) const;

    GaloisFieldElement<p, d>& operator+=(const GaloisFieldElement<p, d>&);
    GaloisFieldElement<p, d>& operator*=(const GaloisFieldElement<p, d>&);
    GaloisFieldElement<p, d>& operator-=(const GaloisFieldElement<p, d>&);
    GaloisFieldElement<p, d>& operator/=(const GaloisFieldElement<p, d>&);

    GaloisFieldElement<p, d>& operator=(const GaloisFieldElement<p, d>&);

    bool operator == (const GaloisFieldElement<p, d>&) const;

    template <int p1, int d1>
    friend std::ostream& operator<<(std::ostream&, const GaloisFieldElement<p1, d1>&);

    static number get_mod();

    GaloisFieldElement<p, d> next() const;
private:
    void init();

    static number MOD;
    number value;
};

#include "galois_field.cpp"
