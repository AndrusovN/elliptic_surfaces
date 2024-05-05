#ifndef PRIME_FIELD_HEADER
#include "prime_field.h"
#endif
#include "math_exception.h"

template<int p>
PrimeFieldElement<p>::PrimeFieldElement() : value(0) {}

template<int p>
PrimeFieldElement<p>::PrimeFieldElement(int v) {
    if (v < 0) {
        v -= p * (-1 + v / p);
    }
    value = v % p;
}

template<int p>
PrimeFieldElement<p> PrimeFieldElement<p>::operator-() const {
    return PrimeFieldElement<p>(-value);
}

template<int p>
PrimeFieldElement<p>::PrimeFieldElement(const PrimeFieldElement<p>& source) : value(source.value) {}

template<int p>
PrimeFieldElement<p>::PrimeFieldElement(PrimeFieldElement<p>&& source) : value(source.value) {
    source.value = -1;
}

template<int p>
bool PrimeFieldElement<p>::operator==(const PrimeFieldElement<p> &other) const {
    return value == other.value;
}

template<int p>
PrimeFieldElement<p> PrimeFieldElement<p>::pow(int n) const {
    PrimeFieldElement<p> temp = *this;
    PrimeFieldElement<p> result(1);
    while(n > 0) {
        if (n & 1) {
            result *= temp;
        }
        temp *= temp;
        n >>= 1;
    }
    return result;
}

template<int p>
PrimeFieldElement<p> PrimeFieldElement<p>::inverse() const {
    if (value == 0) {
        throw MathException("Inversion of a zero element!");
    }
    return pow(p-2);
}

template<int p>
int PrimeFieldElement<p>::to_int() const {
    return value;
}

template <int p>
PrimeFieldElement<p> PrimeFieldElement<p>::operator+(const PrimeFieldElement<p>& other) const {
    auto copy = *this;
    return copy += other;
}

template <int p>
PrimeFieldElement<p> PrimeFieldElement<p>::operator-(const PrimeFieldElement<p>& other) const {
    auto copy = *this;
    return copy -= other;
}

template <int p>
PrimeFieldElement<p> PrimeFieldElement<p>::operator*(const PrimeFieldElement<p>& other) const {
    auto copy = *this;
    return copy *= other;
}

template <int p>
PrimeFieldElement<p> PrimeFieldElement<p>::operator/(const PrimeFieldElement<p>& other) const {
    auto copy = *this;
    return copy /= other;
}


template <int p>
PrimeFieldElement<p>& PrimeFieldElement<p>::operator+=(const PrimeFieldElement<p>& other) {
    value += other.value;
    if (value >= p) value -= p;
    return *this;
}

template <int p>
PrimeFieldElement<p>& PrimeFieldElement<p>::operator*=(const PrimeFieldElement<p>& other) {
    value *= other.value;
    value %= p;
    return *this;
}

template <int p>
PrimeFieldElement<p>& PrimeFieldElement<p>::operator-=(const PrimeFieldElement<p>& other) {
    value -= other.value;
    if (value < 0) value += p;
    return *this;
}

template <int p>
PrimeFieldElement<p>& PrimeFieldElement<p>::operator/=(const PrimeFieldElement<p>& other) {
    value *= other.inverse().value;
    value %= p;
    return *this;
}

template<int p>
PrimeFieldElement<p> &PrimeFieldElement<p>::operator=(const PrimeFieldElement<p> &source) {
    value = source.value;
    return *this;
}

template <int p>
std::ostream& operator<<(std::ostream& out, const PrimeFieldElement<p>& val) {
    return out << val.value;
}
