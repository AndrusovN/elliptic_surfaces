#ifndef POLYNOMIAL_HEADER
#include "polynom.h"
#endif
#include <random>
#include <sstream>
#include "math_exception.h"
#include "prime_field.h"

size_t revert_binary(size_t index, size_t length) {
    size_t result = 0;
    for (size_t i = 0; i < length; ++i) {
        if ((index >> i) & 1) {
            result |= (1 << (length - i - 1));
        }
    }
    return result;
}

template <Field Field_t>
std::vector<c_field> Polynomial<Field_t>::to_complex(size_t target_size) const {
    if (target_size <= coefficients.size()) {
        throw MathException("Trying to convert a polynomial into too small length array");
    }
    size_t result_size = 1;
    while(target_size > 0) {
        result_size *= 2;
        target_size /= 2;
    }
    std::vector<c_field> result(result_size, 0);
    for (size_t i = 0; i < coefficients.size(); ++i) {
        result[i] = static_cast<c_field>(coefficients[i].to_int());
    }

    return result;
}
template<Field Field_t>
void Polynomial<Field_t>::build_from_complex(const std::vector<c_field> &coefs) {
    coefficients.resize(coefs.size(), 0);
    for (size_t i = 0; i < coefficients.size(); ++i) {
        coefficients[i] = Field_t(static_cast<long long>(coefs[i].real() + 0.5));
    }
}

void reorder_for_fft(std::vector<c_field>& source) {
    size_t length = 0;
    while((1u << length) < source.size()) ++length;

    for (size_t i = 0; i < source.size(); ++i) {
        if (i < revert_binary(i, length)) {
            std::swap(source[i], source[revert_binary(i, length)]);
        }
    }
}

void fft(std::vector<c_field>& source, bool inverse) {
    c_field root_coefficient;

    reorder_for_fft(source);

    for (size_t block_length = 2; block_length <= source.size(); block_length *= 2) {
        long double angle = 2 * M_PI / static_cast<long double>(block_length) * (inverse ? -1 : 1);
        root_coefficient = c_field(cosl(angle), sinl(angle));

        for (size_t block_id = 0; block_id < source.size() / block_length; ++block_id) {
            size_t left_part = block_id * block_length;
            size_t right_part = left_part + block_length / 2;

            c_field current_root = 1;

            for (size_t i = 0; i < block_length / 2; ++i) {
                c_field first = source[left_part + i] + current_root * source[right_part + i];
                c_field second = source[left_part + i] - current_root * source[right_part + i];
                source[left_part + i] = first;
                source[right_part + i] = second;
                current_root *= root_coefficient;
            }
        }
    }

    if (inverse) {
        for (size_t i = 0; i < source.size(); ++i) {
            source[i] /= source.size();
        }
    }
}

template<Field Field_t>
std::ostream &operator<<(std::ostream& out, const Polynomial<Field_t>& poly) {
    return out << poly.to_string();
}

template <Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::inverse_polynomial() const {
    if (coefficients.size() == 1) {
        if (coefficients[0] == 0) return Polynomial<Field_t>(Field_t(0));
        return Polynomial<Field_t>(coefficients[0].inverse());
    }
    auto copy = *this;
    Polynomial<Field_t> LeftHalf;
    LeftHalf.coefficients.resize(coefficients.size() / 2);
    Polynomial<Field_t> RightHalf;
    RightHalf.coefficients.resize(coefficients.size() / 2);

    for (int i = 0; i < coefficients.size() / 2; ++i) {
        LeftHalf.coefficients[i] = coefficients[i];
        RightHalf.coefficients[i] = coefficients[i + coefficients.size() / 2];
    }
    auto LeftHalf_inv = LeftHalf.inverse_polynomial();
    auto LeftResult = LeftHalf.multiplied_by(LeftHalf_inv);

    Polynomial<Field_t> Mixed_inv = LeftHalf_inv.multiplied_by(RightHalf);
    for (int i = 0; i < coefficients.size() / 2; ++i) {
        if (i + coefficients.size() / 2 >= LeftResult.coefficients.size()
            || i >= Mixed_inv.coefficients.size()) break;
        Mixed_inv.coefficients[i] += LeftResult.coefficients[i + coefficients.size() / 2];
    }
    Polynomial<Field_t> big_part = LeftHalf_inv.multiplied_by(Mixed_inv);
    Polynomial<Field_t> answer;
    answer.coefficients.resize(coefficients.size(), 0);
    for (int i = 0; i < coefficients.size() / 2; ++i) {
        if (i < LeftHalf_inv.coefficients.size()) {
            answer.coefficients[i] = LeftHalf_inv.coefficients[i];
        }
        if (i < big_part.coefficients.size()) {
            answer.coefficients[i + coefficients.size() / 2] = -big_part.coefficients[i];
        }
    }
    return answer;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::reversed() const {
    Polynomial<Field_t> result;
    result.coefficients.resize(coefficients.size());
    for (int i = 0; i < coefficients.size(); ++i) {
        result.coefficients[i] = coefficients[coefficients.size() - 1 - i];
    }
    return result;
}


template<Field Field_t>
Polynomial<Field_t>::Polynomial(): coefficients(1,0) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(Field_t element): coefficients(1,std::move(element)) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(const Polynomial<Field_t> &source): coefficients(source.coefficients.size()) {
    for (size_t i = 0; i < coefficients.size(); ++i) {
        coefficients[i] = source.coefficients[i];
    }
}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(int element) : Polynomial(Field_t(element)) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(const std::vector<Field_t> &data): coefficients(data) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(Polynomial<Field_t> &&source) noexcept : coefficients(std::move(source.coefficients)) {}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator=(const Polynomial<Field_t> &other) {
    auto copy = other;
    std::swap(copy.coefficients, coefficients);
    return *this;
}

template<Field Field_t>
int Polynomial<Field_t>::degree() const {
    return coefficients.size() - 1;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::operator+(const Polynomial &other) const {
    auto copy = *this;
    return copy += other;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::operator-(const Polynomial &other) const {
    auto copy = *this;
    return copy -= other;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::operator*(const Polynomial &other) const {
    auto copy = *this;
    return copy *= other;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::operator%(const Polynomial &other) const {
    auto copy = *this;
    return copy %= other;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::power(int n) const {
    auto tmp = *this;
    Polynomial<Field_t> result(1);
    while (n > 0) {
        if (n & 1) {
            result *= tmp;
        }
        n >>= 1;
        tmp *= tmp;
    }
    return result;
}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator+=(const Polynomial<Field_t> &other) {
    int max_len = std::max(coefficients.size(), other.coefficients.size());
    for (size_t i = 0; i < max_len; ++i) {
        if (coefficients.size() == i) {
            coefficients.push_back(other.coefficients[i]);
        } else if (other.coefficients.size() > i) {
            coefficients[i] += other.coefficients[i];
        }
    }
    remove_leading_zeroes();
    return *this;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::operator-() const {
    auto copy = *this;
    for (size_t i = 0; i < coefficients.size(); ++i) {
        copy.coefficients[i] = -copy.coefficients[i];
    }
    return copy;
}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator-=(const Polynomial<Field_t> &other) {
    return *this += -other;
}


template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::multiply(const Polynomial<Field_t> &other) {
    auto my_complex_values = to_complex(coefficients.size() + other.coefficients.size());
    fft(my_complex_values, false);

    auto other_complex_values = other.to_complex(coefficients.size() + other.coefficients.size());
    fft(other_complex_values, false);

    for (size_t i = 0; i < my_complex_values.size(); ++i) {
        my_complex_values[i] *= other_complex_values[i];
    }

    fft(my_complex_values, true);
    build_from_complex(my_complex_values);
    return *this;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::multiplied_by(const Polynomial<Field_t> &other) {
    auto copy = *this;
    return copy.multiply(other);
}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator*=(const Polynomial<Field_t> &other) {
    multiply(other);
    remove_leading_zeroes();
    return *this;
}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator%=(const Polynomial<Field_t> &other) {
    auto [result, remainder] = divide_with_remainder(other);
    std::swap(coefficients, remainder.coefficients);
    return *this;
}

template<Field Field_t>
Field_t Polynomial<Field_t>::operator()(const Field_t &value) const {
    Field_t x_power = 1;
    Field_t result = 0;
    for (auto& coefficient : coefficients) {
        result += x_power * coefficient;
        x_power *= value;
    }
    return result;
}

template<Field Field_t>
void Polynomial<Field_t>::remove_leading_zeroes() {
    while(coefficients.size() > 1 && coefficients.back() == 0) coefficients.pop_back();
    if (coefficients.empty()) coefficients.push_back(0);
}

template<Field Field_t>
std::string Polynomial<Field_t>::to_string() const {
    std::stringstream ss;

    for (int i = coefficients.size() - 1; i >= 0; --i) {
        if (coefficients[i].to_int() != 1 || i == 0) ss << coefficients[i].to_int();
        if (i > 0) {
            ss << "x";
            if (i > 1) {
                ss << "^{" << i << "}";
            }
            ss << " + ";
        }
    }
    return ss.str();
}


template<Field Field_t>
std::string Polynomial<Field_t>::to_string_unified() const {
    std::stringstream ss;

    for (int i = coefficients.size() - 1; i >= 0; --i) {
        ss << coefficients[i].to_int();
        ss << "x";
        ss << "^{" << i << "}";
        if (i > 0) {
            ss << "+";
        }
    }
    return ss.str();
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::from_string_unified(const std::string& source) {
    std::vector<Field_t> result;

    std::string coef;
    bool after_plus = true;

    for (char c : source) {
        if (c == 'x') {
            result.push_back(atoi(coef.c_str()));
            coef = "";
            after_plus = false;
        }
        if (after_plus) {
            coef += c;
        }
        if (c == '+') {
            after_plus = true;
        }
    }
    Polynomial<Field_t> result_poly(result);
    return result_poly.reversed();
}

template<Field Field_t>
Field_t Polynomial<Field_t>::get_coefficient(size_t degree) const {
    return coefficients[degree];
}

template<Field Field_t>
bool Polynomial<Field_t>::operator==(const Polynomial<Field_t> &other) const {
    if (degree() != other.degree()) return false;
    for (size_t i = 0; i <= degree(); ++i) {
        if (coefficients[i] != other.coefficients[i]) return false;
    }
    return true;
}

template<Field Field_t>
Polynomial<Field_t> gcd(const Polynomial<Field_t>& a, const Polynomial<Field_t>& b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

template<Field Field_t>
Polynomial<Field_t> gcd_slow(const Polynomial<Field_t>& a, const Polynomial<Field_t>& b) {
    if (b == 0) return a;
    return gcd_slow(b, a.divide_slow(b).second);
}

std::mt19937 poly_random(987654321);
template<Field Field_t>
Polynomial<Field_t> generate_random_polynomial(size_t degree) {
    std::vector<Field_t> coefficients(degree + 1, 0);
    for (int i = 0; i <= degree; ++i) {
        do {
            coefficients[i] = poly_random();
        } while (coefficients[i] == 0);
    }
    return Polynomial<Field_t>(coefficients);
}

// Implement Cantor-Zassenhaus algorithm
// https://en.wikipedia.org/wiki/Cantor%E2%80%93Zassenhaus_algorithm
template<int p>
Polynomial<PrimeFieldElement<p>> get_divisor(const Polynomial<PrimeFieldElement<p>> &source, int d) {
    auto auxiliary = generate_random_polynomial<PrimeFieldElement<p>>(d-1); // b(x)
    auxiliary = auxiliary.power((std::pow(p, d) - 1) / 2);
    auto B = gcd(source, auxiliary + 1);
    auto C = gcd(source, auxiliary - 1);

    if (B.degree() <= 1) return C;
    if (C.degree() <= 1) return B;

    return B.degree() < C.degree() ? B : C;
}


template<Field Field_t>
std::pair<Polynomial<Field_t>, Polynomial<Field_t>> Polynomial<Field_t>::divide_slow(const Polynomial<Field_t> &other) const {
    if (other == 0) throw MathException("Division by zero polynomial");
    Polynomial<Field_t> res = 0;
    auto copy = *this;
    while (copy.degree() >= other.degree()) {
        Field_t coef = copy.coefficients.back() / other.coefficients.back();
        res.coefficients.push_back(coef);
        for (size_t i = 0; i <= other.degree(); ++i) {
            copy.coefficients[copy.degree() - other.degree() + i] -= other.coefficients[i] * coef;
        }
        copy.coefficients.pop_back();
        while(copy.coefficients.back() == 0 && copy.degree() >= other.degree()) {
            res.coefficients.push_back(0);
            copy.coefficients.pop_back();
        }
    }
    copy.remove_leading_zeroes();
    res = res.reversed();
    res.remove_leading_zeroes();
    return std::make_pair(res, copy);
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::multiplied_slow_by(const Polynomial<Field_t> &other) {
    auto copy = *this;
    return copy.multiply_slow(other);
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::multiply_slow(const Polynomial<Field_t> &other) {
    Polynomial<Field_t> result;
    if (*this == 0 || other == 0) return 0;
    result.coefficients.resize(degree() + other.degree() + 1, 0);
    for (size_t i = 0; i < result.coefficients.size(); ++i) {
        for (size_t j = 0; j <= degree(); ++j) {
            if (i - j >= 0 && i - j <= other.degree()) {
                result.coefficients[i] += coefficients[j] * other.coefficients[i - j];
            }
        }
    }
    std::swap(coefficients, result.coefficients);
    return *this;
}

template<Field Field_t>
std::pair<Polynomial<Field_t>, Polynomial<Field_t>>
Polynomial<Field_t>::divide_with_remainder(const Polynomial<Field_t> &other) const {
    Polynomial<Field_t> remainder = *this;
    if (other.degree() > degree()) {
        return std::make_pair(0, *this);
    }
    auto this_reversed = reversed();
    auto other_reversed = other.reversed();
    auto copy = other_reversed;
    while ((copy.coefficients.size() & (copy.coefficients.size() - 1)) != 0 || copy.coefficients.size() < degree() + 1) {
        copy.coefficients.push_back(0);
    }
    auto inv = copy.inverse_polynomial();
    auto mult = this_reversed.multiplied_by(inv);
    int sz = std::max(1, this_reversed.degree() - other_reversed.degree() + 1);
    Polynomial<Field_t> result;
    result.coefficients.resize(sz, 0);
    for (int i = 0; i < sz; ++i) {
        result.coefficients[i] = mult.coefficients[sz - i - 1];
    }
    result.remove_leading_zeroes();
    auto left = result.multiplied_by(other);
    remainder.coefficients.resize(other_reversed.degree(), 0);
    for (int i = 0; i <= other_reversed.degree(); ++i) {
        if (left.coefficients.size() > i) remainder.coefficients[i] -= left.coefficients[i];
    }
    remainder.remove_leading_zeroes();
    return std::make_pair(result, remainder);
}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator/=(const Polynomial<Field_t> &other) {
    auto [result, remainder] = divide_with_remainder(other);
    std::swap(coefficients, result.coefficients);
    return *this;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::operator/(const Polynomial &other) const {
    return divide_with_remainder(other).first;
}

template<Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::next() const {
    auto copy = *this;
    int id = 0;
    do {
        if (id == copy.coefficients.size()) copy.coefficients.push_back(Field_t(0));
        copy.coefficients[id] += 1;
        ++id;
    } while (copy.coefficients[id - 1] == 0);
    return copy;
}
