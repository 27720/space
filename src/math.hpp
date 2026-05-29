// Currently only contains the math needed to simulate a 2D Keplerian orbit with a fixed central body

#include <glm/glm.hpp>

/*
I decided to not use SI units because it makes the numbers look cleaner (robbed this idea from some guy on LinkedIn) 

    - Distances will be measured in Astronomical Units (AU), where 1 AU = 1.496 * 10^11 metres (distance between the Earth and the Sun)
    - Masses will be measured in solar masses (M_s), where 1 M_s = 1.989 * 10^30 kg (mass of the sun)
    - Time will be measured in a sacred unit used by the Babylonians, seconds (s)

Since we're not using standard units, we'll have to work out the value of gravitational constant relative to our units
Gemini says G in these units is roughly 2.959 * 10-4 AU^3 M_s^-1 s^-2 
*/

constexpr double G = 2.959e-4;

/*
A Keplerian orbit refers to a mathematically ideal 2-body orbit
It can be fully described by an orbits A and E  values (given that the orbit is about a fixed central body), where A is the semi-major axis and e is the eccentricity
*/

struct Orbit { // i dont fw capitalising stuff but convention is convention i guess
    double a;
    double e;
};

/*
Kepler's equation relates two angles that describe where in orbit the body is, relative to one of its focal points

M = E - e * sin(E), where M is the mean anomaly and E is the eccentric anomaly (both in radians, kepler shouldnt have been allowed to name this sh)

The mean anomaly is essentially a 'fake angle' that represents where the body WOULD be if it moved at a constant velocity
Its derivation is pretty cool, involves angular velocity

The eccentric anomaly is the angle we actually want, representing where the body actually is

An issue with this equation is that its unsolvable, since you can't make E the subject
As a result, you have to use a numerical method, of which I found Newton-Raphsons to be the best (it's also the only one I know)
Although it's technically an approximation, the methods insanely good and only takes a few iterations to get a
computationally perfect result (not even enuf memory to store a more accurate value)
*/

double solveKepler(double M, double e);

/*
As described earlier, we can convert the eccentric anomaly into an (x, y) coordinate by:
    - Calculating the Orbital period (T) via Kepler's Third Law (K3)
    - Using the T to calculate Mean anomaly (M)
    - Using M to calculate the Eccentric anomaly
    - Using geometry to work out the x and y coordinates (which also has a cool derivation that I spent an hour trying to work out myself)
*/

glm::dvec2 orbitPosition(const Orbit& orbit, double starMass, double t);