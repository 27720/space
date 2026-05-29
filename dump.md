## FINISHING BY 20/6

### NOTES

- dir struct that separates the math and GL, shaders can stay homeless
- doubles instead of floats for precision
- gonna have to use pascalcase for structs cause thats the standard apparently
- implement atleast 3 integrators, ideally 2 stand, 1 sympl
	- verlet, semi-implict eulerian, standard eulerian, rk4
- append simulation data into a csv, then plot onto a graph of system energy against time
	- plot via matplotlib or octave
	- run for an ingame year 
- keplers equation defines a mathematically ideal 2 body orbit
	- $M = E - e \sin(E)$
	- algebraically unsolvable since its impossible to rearrange for E
	- however the newton-raphson approximation yields a computationally 'perfect' value
	- convert to x and y coordinates relative to star COM using $x = a \cos(E) - ae$ and $y = a \sin(E) \sqrt{1 - e^2}$ 
	- assumes a coordinate system relative to the right focal point

### INTEGRATORS

- Standard Eulerian
	- https://en.wikipedia.org/wiki/Euler_method
	- By far the most computationally cheap
	- Loses accuracy very fast
- Runge-Kutta
	- https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
	- Marginally more accurate than Standard Eulerian
	- Testing over elongated periods of time should still see artificial orbital decay 
- Semi-Implict Eulerian
	- https://en.wikipedia.org/wiki/Semi-implicit_Euler_method
	- This is a symplectic integrator, which means its works with a body's geometric properties (phase space volume & energy ) rather than position and velocity
	- Expecting a slightly shakey orbit that has a mean stable orbit (energy bounds)
- Verlet
	- https://en.wikipedia.org/wiki/Verlet_integration
	- Another symplectic method, saw it on LinkedIn in someone's three-body simulation
	
### ABOUT

- the projects initial spark came from an urge to stare at balls in 3D rather than 2D
- it then became about planets, leading me to numerical integrators and orbital mechanics which i had a lot of fun looking into
- eventually turned into an analytical simulation of n bodies orbiting about a star using different methods of integration
- to be specific, the 'star' is a body - it just represents a body that makes up a majority of the systems mass (something like our sun). this is because an orbit happens about the systems centre of mass (the 'barycentre'), so by having a relatively massive body in the screens centre, we restrict the barycentre to be somewhere within that region, which looks nicer to me and the real goal of this project is making a visual lullaby for myself
- should still expect the barycentre to shift due to gravitation
- simulation itself will be ran for a year (in game) for each integration method