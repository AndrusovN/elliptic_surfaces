#ifndef GALOIS_FIELD_HEADER
#include "galois_field.h"
#endif
#include <fstream>
#include <filesystem>

using std::filesystem::path;

template<int p, int d>
GaloisFieldElement<p, d>::number GaloisFieldElement<p, d>::MOD;

template<int p>
Polynomial<PrimeFieldElement<p>> get_prime_polynomial(int d) {
    std::vector<PrimeFieldElement<p>> coefficients(std::pow(p, d));
    coefficients.back() = 1;
    coefficients[0] = -1;
    auto source = Polynomial<PrimeFieldElement<p>>(coefficients);
    auto divisor = get_divisor<p>(source, d);
    while (divisor.degree() > d) {
        auto div = get_divisor<p>(divisor, d);
        divisor = div;
    }
    return divisor;
}

inline auto generate_filename(int p) {
    return path("../") / path("prime_polynomials") / (std::to_string(p) + ".plnm");
}

template <int p>
void prepare_polynomials(int max_degree) {
    std::vector<Polynomial<PrimeFieldElement<p>>> old_records = get_prime_cached_info<p>();
    int old_max_degree = old_records.size() + 1;

    if (old_max_degree >= max_degree) {
        return;
    }

    for (int d = old_max_degree + 1; d <= max_degree; ++d) {
        old_records.push_back(get_prime_polynomial<p>(d));
    }

    std::fstream f_write;
    f_write.open(generate_filename(p), std::fstream::out);
    f_write << max_degree << std::endl;
    for (auto& rec : old_records) {
        f_write << rec.to_string_unified() << std::endl;
    }
    f_write.close();
    std::cout << "Data saved into " << generate_filename(p) << std::endl;
}

template<int p>
std::vector<Polynomial<PrimeFieldElement<p>>> get_prime_cached_info() {
    std::fstream test_existance(generate_filename(p));
    std::ifstream f_read;
    int max_degree;
    std::vector<Polynomial<PrimeFieldElement<p>>> data;
    if (test_existance.good()) {
        if (test_existance.is_open()) test_existance.close();
        f_read.open(generate_filename(p), std::fstream::in);
        f_read >> max_degree;
        for (int i = 2; i <= max_degree; ++i) {
            std::string poly;
            f_read >> poly;
            data.push_back(Polynomial<PrimeFieldElement<p>>::from_string_unified(poly));
        }
        f_read.close();
    }
    return data;
}

template<int p>
Polynomial<PrimeFieldElement<p>> get_prime_polynomial_cached(int d) {
    prepare_polynomials<p>(d);
    auto data = get_prime_cached_info<p>();
    return data[d - 2];
}

template<Field Field_t>
std::pair<Polynomial<Field_t>, Polynomial<Field_t>>
extended_euclidean_algorithm(const Polynomial<Field_t>& A, const Polynomial<Field_t>& B) {
    if (B == 0) {
        return std::make_pair(Polynomial<Field_t>(1), Polynomial<Field_t>(0));
    }
    auto [result, remainder] = A.divide_with_remainder(B);
    auto [alpha, beta] = extended_euclidean_algorithm(B, remainder);
    return std::make_pair(beta, alpha - beta * result);
}


template<int p, int d>
void GaloisFieldElement<p, d>::init() {
    if (d == 1) {
        MOD = Polynomial<PrimeFieldElement<p>>({0, 1});
    } else {
        GaloisFieldElement<p, d>::MOD = get_prime_polynomial_cached<p>(d);
    }
}

template<int p, int d>
GaloisFieldElement<p, d>::GaloisFieldElement(): value(0) {
    if (MOD == 0) init();
}

template<int p, int d>
GaloisFieldElement<p, d>::GaloisFieldElement(GaloisFieldElement::number val): value(val) {
    if (MOD == 0) init();
}


template<int p1, int d1>
std::ostream &operator<<(std::ostream& out, const GaloisFieldElement<p1, d1>& element) {
    return out << element.value;
}

template<int p, int d>
bool GaloisFieldElement<p, d>::operator==(const GaloisFieldElement<p, d>& other) const {
    return value == other.value;
}

template<int p, int d>
GaloisFieldElement<p, d> &GaloisFieldElement<p, d>::operator/=(const GaloisFieldElement<p, d>& other) {
    return *this *= other.inverse();
}

template<int p, int d>
GaloisFieldElement<p, d> &GaloisFieldElement<p, d>::operator-=(const GaloisFieldElement<p, d>& other) {
    value -= other.value;
    return *this;
}

template<int p, int d>
GaloisFieldElement<p, d> &GaloisFieldElement<p, d>::operator*=(const GaloisFieldElement<p, d>& other) {
    value *= other.value;
    value %= MOD;
    return *this;
}

template<int p, int d>
GaloisFieldElement<p, d> &GaloisFieldElement<p, d>::operator+=(const GaloisFieldElement<p, d>& other) {
    value += other.value;
    return *this;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::operator/(const GaloisFieldElement<p, d>& other) const {
    auto copy = *this;
    return copy /= other;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::operator*(const GaloisFieldElement<p, d>& other) const {
    auto copy = *this;
    return copy *= other;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::operator-(const GaloisFieldElement<p, d>& other) const {
    auto copy = *this;
    return copy -= other;
}

template<int p, int d>
GaloisFieldElement<p, d> &GaloisFieldElement<p, d>::operator=(const GaloisFieldElement<p, d>& other) {
    value = other.value;
    return *this;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::operator+(const GaloisFieldElement<p, d>& other) const {
    auto copy = *this;
    return copy += other;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::operator-() const {
    return GaloisFieldElement<p, d>(-value);
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::pow(int power) const {
    GaloisFieldElement result = GaloisFieldElement(1);
    GaloisFieldElement current_pow = *this;

    while(power > 0) {
        if (power & 1) {
            result *= current_pow;
        }
        current_pow *= current_pow;
        power >>= 1;
    }
    return result;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::inverse() const {
    auto [_, result] = extended_euclidean_algorithm(MOD, value);
    auto res = result * value;
    res %= MOD;
    result *= number(res.get_coefficient(0).inverse());
    return result;
}

template<int p, int d>
GaloisFieldElement<p, d>::number GaloisFieldElement<p, d>::get_mod() {
    return MOD;
}

template<int p, int d>
GaloisFieldElement<p, d> GaloisFieldElement<p, d>::next() const {
    auto poly = value.next();
    if (poly.degree() == d) poly = 0;
    return GaloisFieldElement<p, d>(poly);
}
