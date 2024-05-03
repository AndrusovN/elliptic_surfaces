#ifndef POLYNOM_HEADER
#include "polynom.h"
#endif
#include <sstream>
#include "math_exception.h"

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
    remove_leading_zeroes();
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

template <Field Field_t>
Polynomial<Field_t> Polynomial<Field_t>::inverse_polynomial() const {
    if (coefficients.size() == 1) {
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
    auto LeftResult = LeftHalf * LeftHalf_inv;

    Polynomial<Field_t> Mixed_inv = LeftHalf_inv * RightHalf;
    for (int i = 0; i < coefficients.size() / 2; ++i) {
        Mixed_inv.coefficients[i] += LeftResult.coefficients[i + coefficients.size() / 2];
    }
    Polynomial<Field_t> big_part = LeftHalf_inv * Mixed_inv;
    Polynomial<Field_t> answer;
    answer.coefficients.resize(coefficients.size(), 0);
    for (int i = 0; i < coefficients.size() / 2; ++i) {
        answer.coefficients[i] = LeftHalf_inv.coefficients[i];
        answer.coefficients[i + coefficients.size() / 2] = -big_part.coefficients[i];
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
Polynomial<Field_t>::Polynomial(): coefficients(1,1) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(Field_t element): coefficients(1,std::move(element)) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(const Polynomial<Field_t> &source): coefficients(source.coefficients.size()) {
    for (size_t i = 0; i < coefficients.size(); ++i) {
        coefficients[i] = source.coefficients[i];
    }
}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(const std::vector<Field_t> &data): coefficients(data) {}

template<Field Field_t>
Polynomial<Field_t>::Polynomial(Polynomial<Field_t> &&source) noexcept : coefficients(std::move(source.coefficients)) {}


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
Polynomial<Field_t> &Polynomial<Field_t>::operator*=(const Polynomial<Field_t> &other) {
    auto my_complex_values = to_complex(coefficients.size() + other.coefficients.size());
    fft(my_complex_values, false);

    auto other_complex_values = other.to_complex(coefficients.size() + other.coefficients.size());
    fft(other_complex_values, false);

    for (size_t i = 0; i < my_complex_values.size(); ++i) {
        my_complex_values[i] *= other_complex_values[i];
    }

    fft(my_complex_values, true);
    build_from_complex(my_complex_values);
    remove_leading_zeroes();
    return *this;
}

template<Field Field_t>
Polynomial<Field_t> &Polynomial<Field_t>::operator%=(const Polynomial<Field_t> &other) {
    if (other.degree() > degree()) return *this;

    auto copy = other;
    while ((copy.coefficients.size() & (copy.coefficients.size() - 1)) != 0) copy.coefficients.push_back(0);
    auto inv = copy.inverse_polynomial();
    auto mult = *this * inv;
    int sz = std::max(1, degree() - other.degree() + 1);
    Polynomial<Field_t> result;
    result.coefficients.resize(sz, 0);
    for (int i = 0; i < sz; ++i) {
        result.coefficients[i] = mult.coefficients[sz - i - 1];
    }
    auto me_reversed = reversed();
    auto other_reversed = other.reversed();
    auto left = result * other_reversed;
    coefficients.resize(other.degree());
    for (int i = 0; i <= other.degree(); ++i) {
        coefficients[i] = me_reversed.coefficients[i] - left.coefficients[i];
    }
    remove_leading_zeroes();
    return *this;
}

template<Field Field_t>
void Polynomial<Field_t>::remove_leading_zeroes() {
    while(coefficients.size() > 1 && coefficients.back() == 0) coefficients.pop_back();
}

template<Field Field_t>
std::string Polynomial<Field_t>::to_string() const {
    std::stringstream ss;

    for (int i = coefficients.size() - 1; i >= 0; --i) {
        ss << coefficients[i].to_int() << "x^{" << i << "}";
        if (i > 0) ss << " + ";
    }
    return ss.str();
}
