#ifndef MULTIVARIABLE_POLYNOMIAL_HEADER
#include "multivariable_polynomial.h"
#endif

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>
        MultivariablePolynomial<variables_count, coefficient_t>::operator-() const {
    auto new_coefficients = coefficients;
    for (auto it = new_coefficients.begin(); it != new_coefficients.end(); ++it) {
        it->second = -it->second;
    }
    return MultivariablePolynomial<variables_count, coefficient_t>(new_coefficients);
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::MultivariablePolynomial(
        std::map<MultivariableMonomial<variables_count>, coefficient_t> coeffs): coefficients(std::move(coeffs)) {}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t> &
MultivariablePolynomial<variables_count, coefficient_t>::operator-=
        (const MultivariablePolynomial<variables_count, coefficient_t> &other) {
    return *this += -other;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::operator-
        (const MultivariablePolynomial<variables_count, coefficient_t> &other) const {
    auto copy = *this;
    return copy -= other;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t> &
MultivariablePolynomial<variables_count, coefficient_t>::operator+=
        (const MultivariablePolynomial<variables_count, coefficient_t> &other) {
    for (auto it = other.coefficients.begin(); it != other.coefficients.end(); ++it) {
        coefficients[it->first] += it->second;
    }
    return *this;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::operator+
        (const MultivariablePolynomial<variables_count, coefficient_t> &other) const {
    auto copy = *this;
    return copy += other;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t> &
MultivariablePolynomial<variables_count, coefficient_t>::operator*=
        (const MultivariablePolynomial<variables_count, coefficient_t> &other) {

    return *this = *this * other;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::operator*(const MultivariablePolynomial<variables_count, coefficient_t> &other) const {
    std::map<MultivariableMonomial<variables_count>, coefficient_t> new_coefficients;
    for (auto& [my_mono, my_coef] : coefficients) {
        for (auto& [other_mono, other_coef] : other.coefficients) {
            new_coefficients[my_mono * other_mono] += my_coef * other_coef;
        }
    }
    return MultivariablePolynomial<variables_count, coefficient_t>(new_coefficients);
}

template<int variables_count, typename coefficient_t>
template<Field Field_t>
Field_t MultivariablePolynomial<variables_count, coefficient_t>::operator()(const std::array<Field_t, variables_count> &arguments) const {
    Field_t result = Field_t(0);
    for (auto& [monomial, coefficient]: coefficients) {
        result += monomial(arguments) * Field_t(coefficient);
    }
    return result;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::MultivariablePolynomial(
        const MultivariableMonomial<variables_count> &source) {
    coefficients[source] = 1;
}

template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::MultivariablePolynomial() {}

template<int variables_count, typename coefficient_t>
std::ostream &operator<<(std::ostream &out, const MultivariablePolynomial<variables_count, coefficient_t> &poly) {
    for (auto& [monomial, coefficient] : poly.coefficients) {
        out << coefficient << monomial << "+";
    }
    out << "0";
    return out;
}

template<int variables_count, typename coefficient_t>
std::string MultivariablePolynomial<variables_count, coefficient_t>::to_string() const {
    std::stringstream ss;
    ss << *this;
    std::string result;
    ss >> result;
    return result;
}

template<int variables_count, typename coefficient_t>
bool MultivariablePolynomial<variables_count, coefficient_t>::operator==(
        const MultivariablePolynomial<variables_count, coefficient_t> &other) const {
    auto other_it = other.coefficients.begin();
    for (auto it = coefficients.begin(); it != coefficients.end(); ++it) {
        if (it->second == 0) continue;
        while(other_it != other.coefficients.end() && other_it->second == 0) ++other_it;
        if (other_it == other.coefficients.end()) return false;
        if (other_it->first != it->first) return false;
        if (other_it->second != it->second) return false;
        ++other_it;
    }
    while(other_it != other.coefficients.end() && other_it->second == 0) ++other_it;
    if (other_it != other.coefficients.end()) return false;
    return true;
}


template<int variables_count, typename coefficient_t>
MultivariablePolynomial<variables_count, coefficient_t>::MultivariablePolynomial(coefficient_t C) {
    MultivariableMonomial<variables_count> mono;
    coefficients[mono] = C;
}
