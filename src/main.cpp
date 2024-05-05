#include <filesystem>
#include "surface.h"

using std::filesystem::path;
#define console std::cout

int main() {
    auto folder_to_save = path("..") / "surfaces_points";

    MultivariablePolynomial<3> Poly = MultivariablePolynomial<3>(x * y) - x*x*y - x*y*y - t;
    Surface C(Poly);

    C.print_points<23, 1>(console);
    C.print_points<5, 2>(console);
    C.print_points<7, 2>(console);
    C.print_points<11, 2>(console);
    C.print_points<5, 3>(console);
}
