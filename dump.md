## PLANNING ON FINISHING THIS BY 20/6
- wanna move onto other more ambitious projects that I've had to vault cause physics clears every alvl 

### NOTES

- going for a dir struct where the math and GL stuff are separate, but i doubt ill give my shaders a home
- ideally need to implement 3 integrators
	- likely gonna be the 3 ive already studied (listed)
	- might swap out verlet for standard eulerian since most symplectic integrators have similar energy error bounds
- learnt all the math and a decent amount of GL, so that hopefully means faster progress
- logging simulation results into a csv that logs position and velocity for every dt, and then plotting it on a graph of sys energy against time
	- probably via python bs and matplotlib
	- could figure out how to do it w octave though cause its cool as fuck
	
### INTEGRATORS

- Runge-Kutta
	- https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
	- Marginally more accurate than Standard Eulerian
	- Testing over elongated periods of time should still see artificial orbital decay 
- Eulerian
	- https://en.wikipedia.org/wiki/Semi-implicit_Euler_method
	- This is a symplectic integrator, which means its works with a body's geometric properties (phase space volume & energy ) rather than position and velocity
	- Expecting a slightly shakey orbit that has a mean stable orbit (energy bounds)
- Verlet
	- https://en.wikipedia.org/wiki/Verlet_integration
	- Another symplectic method, saw it on LinkedIn in someone's three-body simulation
	
### ABOUT

- the projects initial spark came from an urge to stare at balls in 3d rather than 2d
- it then became about planets, leading me to numerical integrators and orbital mechanics which i had a lot of fun looking into
- eventually turned into an analytical simulation of n bodies orbiting about a star using different methods of integration
- to be specific, the 'star' is a body. it just represents a body that makes up a majority of the systems mass (something like our sun). this is cause an orbit is about the systems centre of mass (the 'barycentre'), so by having a relatively massive body in the centre, we restrict the barycentre to be somewhere within that region - point being that it looks nicer to me and the real goal of this project is making a visual lullaby for myself. note that the barycentre is still gonna shift due to newtons law of gravitation, a star only acts to mitigate the visual effect of that
- simulation itself will be ran for a year (in game) for each integration method, wit data logged to a csv thatll be externally plotted and analysed. said grafs will then b posted somewhere on the repo
