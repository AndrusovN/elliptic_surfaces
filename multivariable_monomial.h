#pragma once
#define MULTIVARIABLE_MONOMIAL_HEADER
#include <array>
#include <iostream>
#include "field_concept.h"

template <int variables_count>
class MultivariableMonomial {
public:
    MultivariableMonomial();
    MultivariableMonomial(const std::array<int, variables_count>& pows);

    template<Field Field_t>
    Field_t operator() (const std::array<Field_t, variables_count>& arguments) const;

    MultivariableMonomial<variables_count> operator*(const MultivariableMonomial<variables_count>& other) const;
    MultivariableMonomial<variables_count>& operator*=(const MultivariableMonomial<variables_count>& other);

    static MultivariableMonomial<variables_count> X(int id);

    template <int vars>
    friend std::ostream& operator<<(std::ostream&, const MultivariableMonomial<vars>&);

    std::string to_string() const;

    auto operator<=>(const MultivariableMonomial<variables_count>& other) const;

    bool operator==(const MultivariableMonomial<variables_count>& other) const;
private:
    std::array<int, variables_count> powers;
};


#include "multivariable_monomial.cpp"
