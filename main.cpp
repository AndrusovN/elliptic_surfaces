#include <iostream>
#include "polynom.h"
#include "prime_field.h"

int main() {
    Polynomial<PrimeFieldElement<5>> p({1, 4, 3, 1});
    Polynomial<PrimeFieldElement<5>> q({1, 2, 1});

    std::cout << (p + q).to_string() << std::endl;
    std::cout << (p - q).to_string() << std::endl;
    std::cout << (p * q).to_string() << std::endl;
    std::cout << (p % q).to_string() << std::endl;
    std::cout << (q.power(3)).to_string() << std::endl;
    std::cout << p.power(179179).degree() << std::endl;
}
