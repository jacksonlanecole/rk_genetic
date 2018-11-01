# rkev

When analyzing physical systems, there are often cases when analytical
solutions to ordinary differential equations describing the systems
are either exceedingly difficult to find or simply do not exist.
In these cases, it is advantageous to use numerical methods to
iteratively solve the systems. While there are several methods for solving ordinary
differential equations, the "workhorse" of IVP solvers is
the fourth-order Runge-Kutta (RK4).

Essentially, the RK4 method involves computing derivatives at
four different points in the parameter space, and updating the solution at the
iterated parameter to take into account the "weighted average"
of the computed derivatives. Typically, for explicit *s*-stage Runge-Kutta
routines, the entirety of the scheme can be abstracted as a strictly
lower-triangular matrix known as the Butcher Tableau.

Although the Runge-Kutta method is much more accurate than lower order methods
or methods like Euler, an issue that arises in the study of physical
systems is that we are often more aware of the conditions of our system than is
our integrator. When we look at systems in celestial mechanics, like the
three-body problem that has no analytical solution, we should
acknowledge that under the assumption that the system is closed, the total
mechanical energy should remain constant. Operating with this
knowledge, the validity of the solution provided by the RK4 must be called into
question as the total energy is not conserved over long time
integrations.

When modeling physical systems, a common method of of preserving conserved
quantities is to use a class of integrators known as symplectic
integrators. While the scope of this project does not necessarily extend
to symplectic integrators outside of the possible implementation of one for
comparison of the validity of this method, they should recieve some discussion.
In general, many physical systems can be described in the Hamiltonian
formulation of mechanics; this is a far more flexible approach to mechanics than
is Newtonian or even Lagrangian formulation, especially in terms of the
generalization of coordinates and the conservation of naturally conserved
quantities. Symplectic integrators iterate in the solution space of the
resulting Hamiltonian equations of motion, which means that the numerical
solution still respects the conserved quanitites in the physical system.

While symplectic integrators are the go-to choice for numerically solving
systems in which a Hamiltonian can be defined, Runge-Kutta methods are in wider
use as a general-purpose integrator.
This project assumes that the Runge-Kutta method should be able to be optimized
such that conserved
quantities in *any* system would be respected. To be more general, in any
system in which a cost function can be defined, the goal is to
develop a method that improves the results of Runge-Kutta on a system-specific
basis.
The primary directive of this project is to explore the use of evolutionary
algorithms to to improve the results of Runge-Kutta integrators by evolving
the Butcher Tableau describing an *s*-stage Runge-Kutta.

## Getting Started


### Prerequisites
```
cmake>=3.12.1
python3.7
deap==1.2.2
```

### Installing
Setting up the Python virtual environment and activating the virtual environment
can be done by typing
```
virtualenv env
source env/bin/activate
pip install -r requirements.txt
```

The integrators can be built with `make`, and the compiled binaries will be
found in `bin`.

**NOTE:** At this point, the working build does essentially nothing useful, as
it is just testing the RKIntegrator methods. The project can also be built using
cmake in the `build` directory, but I would advise against it at this point.



## Built With

* [DEAP](https://github.com/DEAP/deap) - To be used for all evolutionary
  programming

## Authors

* **Jackson Cole** - [jacksoncole.io](http://jacksoncole.io)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments
