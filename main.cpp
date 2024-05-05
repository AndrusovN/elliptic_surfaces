#include <iostream>
#include "polynom.h"
#include "prime_field.h"
#include "galois_field.h"
#include "surface.h"

#define console std::cout

int main() {
    MultivariablePolynomial<3> Poly = MultivariablePolynomial<3>(x * y) - x*x*y - x*y*y - t;
    Surface C(Poly);
    C.print_points<5, 2>(console);
    C.print_points<7, 2>(console);
    C.print_points<11, 2>(console);
}
