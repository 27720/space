// All functions are explained in math.hpp

#include "math.hpp"
#include <cmath>

using namespace std;

constexpr double PI = 3.14159265358979323846; // 264338327950

double solveKepler(double M, double e) {
    double E = 3.22; // w guess

    for (int i = 0; i < 100; ++i) {
        double f  = E - e * sin(E) - M; 
        double fp = 1.0 - e * cos(E);
        double dE = f / fp;
        E -= dE;

        if (abs(dE) < 1e-10) break; // 
    }

    return E;
}

glm::dvec2 orbitPosition(const Orbit& orbit, double starMass, double t) {

    // Determine Orbital period using Kepler's Third Law
    double T = 2.0 * PI * sqrt( (orbit.a * orbit.a * orbit.a) / (G * starMass) );

    // Determine Mean anomaly
    double M = 2.0 * PI * t / T;
 
    // Determine Eccentric anomaly
    double E = solveKepler(M, orbit.e);

    // Work out Cartesian position

    double b = orbit.a * sqrt(1.0 - orbit.e * orbit.e); // semi-minor axis

    double x = orbit.a * cos(E) - orbit.a * orbit.e;
    double y = b * sin(E);

    return glm::dvec2(x, y);
}