#pragma once
#define SURFACE_HEADER
#include "multivariable_polynomial.h"
#include "galois_field.h"

template <Field Field_t>
struct Point {
    std::array<Field_t, 3> coordinates;
};

template <Field Field_t>
std::ostream& operator<<(std::ostream&, const Point<Field_t>&);

class Surface {
public:
    Surface(const MultivariablePolynomial<3, int>& constraint);

    template <int p, int d>
    std::vector<Point<GaloisFieldElement<p, d>>> find_points() const;

    template <int p, int d>
    void print_points(std::ostream&) const;
private:
    MultivariablePolynomial<3, int> constraint;
};



const MultivariableMonomial<3> x = MultivariableMonomial<3>::X(0);
const MultivariableMonomial<3> y = MultivariableMonomial<3>::X(1);
const MultivariableMonomial<3> t = MultivariableMonomial<3>::X(2);

#include "surface.cpp"