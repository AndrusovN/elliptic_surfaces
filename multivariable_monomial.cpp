#ifndef MULTIVARIABLE_MONOMIAL_HEADER
#include "multivariable_monomial.h"
#endif


template<int variables_count>
MultivariableMonomial<variables_count> MultivariableMonomial<variables_count>::X(int id) {
    std::array<int, variables_count> d;
    for (int i = 0; i < variables_count; ++i) {
        d[i] = i == id ? 1 : 0;
    }
    return MultivariableMonomial<variables_count>(d);
}

template<int variables_count>
MultivariableMonomial<variables_count> &
MultivariableMonomial<variables_count>::operator*=(const MultivariableMonomial<variables_count> &other) {
    for (int i = 0; i < variables_count; ++i) {
        powers[i] += other.powers[i];
    }
    return *this;
}

template<int variables_count>
MultivariableMonomial<variables_count>
MultivariableMonomial<variables_count>::operator*(const MultivariableMonomial<variables_count> &other) const {
    auto copy = *this;
    return copy *= other;
}

template<int variables_count>
template<Field Field_t>
Field_t MultivariableMonomial<variables_count>::operator()(const std::array<Field_t, variables_count> &arguments) const {
    Field_t result(1);
    for (int i = 0; i < variables_count; ++i) {
        result *= arguments[i].pow(powers[i]);
    }
    return result;
}

template<int variables_count>
MultivariableMonomial<variables_count>::MultivariableMonomial(const std::array<int, variables_count> &pows): powers(pows) {}

template<int variables_count>
MultivariableMonomial<variables_count>::MultivariableMonomial() {
    powers.fill(0);
}

template <int variables_count>
std::ostream &operator<<(std::ostream &out, const MultivariableMonomial<variables_count> &monomial) {
    for (int i = 0; i < variables_count; ++i) {
        out << "x_{" << i + 1 << "}^{" << monomial.powers[i] << "}";
    }
    return out;
}

template<int variables_count>
auto MultivariableMonomial<variables_count>::operator<=>(const MultivariableMonomial<variables_count> &other) const {
    for (int i = 0; i < variables_count; ++i) {
        if ((powers[i] <=> other.powers[i]) != std::strong_ordering::equal) {
            return powers[i] <=> other.powers[i];
        }
    }
    return std::strong_ordering::equal;
}

template<int variables_count>
std::string MultivariableMonomial<variables_count>::to_string() const {
    std::stringstream ss;
    ss << *this;
    std::string result;
    ss >> result;
    return result;
}

template<int variables_count>
bool MultivariableMonomial<variables_count>::operator==(const MultivariableMonomial<variables_count> &other) const {
    return (*this <=> other) == std::strong_ordering::equal;
}
