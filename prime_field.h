#pragma once
#define PRIME_FIELD_HEADER
#include <iostream>

template <int p>
class PrimeFieldElement {
public:
    PrimeFieldElement();
    PrimeFieldElement(int);
    PrimeFieldElement(const PrimeFieldElement<p>&);
    PrimeFieldElement(PrimeFieldElement<p>&&);

    PrimeFieldElement<p> inverse() const;
    PrimeFieldElement<p> pow(int n) const;
    PrimeFieldElement<p> operator-() const;

    PrimeFieldElement<p> operator+(const PrimeFieldElement<p>&) const;
    PrimeFieldElement<p> operator-(const PrimeFieldElement<p>&) const;
    PrimeFieldElement<p> operator*(const PrimeFieldElement<p>&) const;
    PrimeFieldElement<p> operator/(const PrimeFieldElement<p>&) const;

    PrimeFieldElement<p>& operator+=(const PrimeFieldElement<p>&);
    PrimeFieldElement<p>& operator*=(const PrimeFieldElement<p>&);
    PrimeFieldElement<p>& operator-=(const PrimeFieldElement<p>&);
    PrimeFieldElement<p>& operator/=(const PrimeFieldElement<p>&);

    PrimeFieldElement<p>& operator=(const PrimeFieldElement<p>&);

    bool operator == (const PrimeFieldElement<p>&) const;

    int to_int() const;
private:
    int value;

    friend std::ostream& operator<<(std::ostream&, const PrimeFieldElement<p>&);
};

template <int p>
std::ostream& operator<<(std::ostream&, const PrimeFieldElement<p>&);

#include "prime_field.cpp"
