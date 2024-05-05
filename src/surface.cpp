#ifndef SURFACE_HEADER
#include "surface.h"
#endif

template<Field Field_t>
std::ostream &operator<<(std::ostream& out, const Point<Field_t>& pt) {
    out << "X = " << pt.coordinates[0] << ", Y = " << pt.coordinates[1] << ", T = " << pt.coordinates[2] << ";";
    return out;
}

Surface::Surface(const MultivariablePolynomial<3, int> &constraint): constraint(constraint) {}

template<int p, int d>
void Surface::print_points(const std::string &folder) const {
    std::ofstream file;
    std::string filename = (path(folder) / (std::to_string((int)std::pow(p, d)) + ".pts")).string();
    file.open(filename, std::ios_base::out);
    print_points<p, d>(file);
    file.close();
    std::cout << "Saved into " << filename << std::endl;
}

template<int p, int d>
void Surface::print_points(std::ostream& out) const {
    auto points = find_points<p, d>();
    out << std::endl;
    out << "Found " << points.size() << " points over Galois Field F_{"
        << std::pow(p, d) << "} = F_{" << p << "^{" << d << "}}" << std::endl << "Here they are:" << std::endl;
    for (auto& pt : points) {
        out << pt << std::endl;
    }
    out << std::endl;
    out << "The result was obtained using F_{" << std::pow(p, d) << "} isomorphism with F_{" << p << "}[x] factorized by ("
        << GaloisFieldElement<p, d>::get_mod() << ")" << std::endl << std::endl;
}

template<int p, int d>
std::vector<Point<GaloisFieldElement<p, d>>> Surface::find_points() const {
    std::vector<Point<GaloisFieldElement<p, d>>> answer;
    std::array<GaloisFieldElement<p, d>, 3> args =
            {GaloisFieldElement<p, d>(0), GaloisFieldElement<p, d>(0), GaloisFieldElement<p, d>(0)};
    do {
        do {
            do {
                if (constraint(args) == GaloisFieldElement<p, d>(0)) {
                    answer.push_back({args});
                }
                args[2] = args[2].next();
            } while (args[2] != GaloisFieldElement<p, d>(0));
            args[1] = args[1].next();
        } while (args[1] != GaloisFieldElement<p, d>(0));
        args[0] = args[0].next();
    } while (args[0] != GaloisFieldElement<p, d>(0));

    return answer;
}
