#!/usr/bin/env python3
"""Provides System class for use in nbody simulations
"""

#import multiprocessing as mp
import numpy as np

import rk_ext

__author__ = "Jackson Cole"
__credits__ = ["Jackson Cole"]
__license__ = "MIT"
__version__ = "1.0"
__maintainer__ = "Jackson Cole"
__email__ = "me@jacksoncole.io"
__status__ = "Production"

class System:
    """System encapsulates all data and methods relating to an
    n-body system.
    """
    def __init__(self, body_list):
        """
        TODO:
            [ ] Implement a method to cache the original parameters.
            [ ] Implement a "reset" method that resets the system to the
            original parameters.
            [ ] Move definition of **_integrator* to __init__ and develop method
            to reset data and accept a new butcher tableau.
        """
        self.iterator = 0
        self.multi = False
        self.printer = False
        self.consts = {'G':(6.67408e-11)*(3600**2)}
        self.body_list = body_list
        self.num_bodies = len(body_list)
        self.masses = [body.mass for body in self.body_list]
        self.mg_list = [self.consts['G']*mass for mass in self.masses]

        self.x_positions = self.set_positions('x')
        self.y_positions = self.set_positions('y')
        self.z_positions = self.set_positions('z')
        self.x_velocities = self.set_velocities('x')
        self.y_velocities = self.set_velocities('y')
        self.z_velocities = self.set_velocities('z')
        self.radii = self.set_radii()

        self.acceleration_coeffs = self.set_acceleration_coeffs()

        self.energy = [self.get_energy()]
        self.energy_change = {}
        self.energy_change['absolute'] = None
        self.energy_change['percentage'] = None

        self.data = {
            'vx':[[v] for v in self.x_velocities],
            'vy':[[v] for v in self.y_velocities],
            'vz':[[v] for v in self.z_velocities],
            'x':[[p] for p in self.x_positions],
            'y':[[p] for p in self.y_positions],
            'z':[[p] for p in self.z_positions],
            'ac':[[ac] for ac in self.acceleration_coeffs],
        }

    def update_energy_change(self):
        self.energy_change['absolute'] = self.energy[-1] - self.energy[0]
        self.energy_change['percentage'] = \
                (self.energy[-1] - self.energy[0])/abs(self.energy[0])*100

    def setup_integrators(self, tableau, t_init, t_fin, steps):
        self.stepsize = (t_fin - t_init)/steps
        self.vx_integrator = rk_ext.RKIntegrator(
            self.compute_x_acceleration,
            tableau,
            steps,
            t_init,
            t_fin,
            True
        )
        self.vy_integrator = rk_ext.RKIntegrator(
            self.compute_y_acceleration,
            tableau,
            steps,
            t_init,
            t_fin,
            True
        )
        self.vz_integrator = rk_ext.RKIntegrator(
            self.compute_z_acceleration,
            tableau,
            steps,
            t_init,
            t_fin,
            True
        )
        self.x_integrator = rk_ext.RKIntegrator(
            self.get_x_velocity,
            tableau,
            steps,
            t_init,
            t_fin,
            True
        )
        self.y_integrator = rk_ext.RKIntegrator(
            self.get_y_velocity,
            tableau,
            steps,
            t_init,
            t_fin,
            True
        )
        self.z_integrator = rk_ext.RKIntegrator(
            self.get_z_velocity,
            tableau,
            steps,
            t_init,
            t_fin,
            True
        )

    def run(self):
        """
        TODO:
            [ ] Determine if using the RKIntegrator is the best method for
            iterating the positions. I kind of think that there might be an
            error here, but I can't sort it out.
            [ ] Do something about the huge section of commented code here. It's
            an implementation of some multiprocessing features that is mega
            buggy as of right now. I'm not convinced it's even appropriate.
        """
        i = 0
        while not self.vz_integrator.isFinished():
            self.x_velocities = self.vx_integrator.step(self.x_velocities)
            self.y_velocities = self.vy_integrator.step(self.y_velocities)
            self.z_velocities = self.vz_integrator.step(self.z_velocities)

            self.x_positions = self.x_integrator.step(self.x_positions)
            self.y_positions = self.y_integrator.step(self.y_positions)
            self.z_positions = self.z_integrator.step(self.z_positions)

            #for i in range(self.num_bodies):
            #    print(self.x_velocities[i])
            #    self.x_positions[i] = self.x_positions[i] + self.x_velocities[i]*self.stepsize
            #    self.y_positions[i] = self.y_positions[i] + self.y_velocities[i]*self.stepsize
            #    self.z_positions[i] = self.z_positions[i] + self.z_velocities[i]*self.stepsize

            self.acceleration_coeffs = self.set_acceleration_coeffs()
            self.update_data()
            self.energy.append(self.get_energy())
            self.update_energy_change()

            #if self.multi: #    vx = mp.Process(
            #        target=self.update_velocities,
            #        args=(self.vx_integrator.step(self.x_velocities), 'x')
            #    )
            #    vy = mp.Process(
            #        target=self.update_velocities,
            #        args=(self.vy_integrator.step(self.y_velocities), 'y')
            #    )
            #    vz = mp.Process(
            #        target=self.update_velocities,
            #        args=(self.vz_integrator.step(self.z_velocities), 'z')
            #    )
            #    x = mp.Process(
            #        target=self.update_positions,
            #        args=(self.x_integrator.step(self.x_positions), 'x')
            #    )
            #    y = mp.Process(
            #        target=self.update_positions,
            #        args=(self.y_integrator.step(self.y_positions), 'y')
            #    )
            #    z = mp.Process(
            #        target=self.update_positions,
            #        args=(self.z_integrator.step(self.z_positions), 'z')
            #    )

            #    vx.start()
            #    vy.start()
            #    vz.start()
            #    vx.join()
            #    vy.join()
            #    vz.join()

            #    x .start()
            #    y .start()
            #    z .start()
            #    x .join()
            #    y .join()
            #    z .join()

            #else:
            #    #self.update_velocities(self.vx_integrator.step(self.x_velocities), 'x')
            #    #self.update_velocities(self.vy_integrator.step(self.y_velocities), 'y')
            #    #self.update_velocities(self.vz_integrator.step(self.z_velocities), 'z')
            #    #self.update_positions(self.x_integrator.step(self.x_positions), 'x')
            #    #self.update_positions(self.y_integrator.step(self.y_positions), 'y')
            #    #self.update_positions(self.z_integrator.step(self.z_positions), 'z')

            #    self.x_velocities = self.vx_integrator.step(self.x_velocities)
            #    self.y_velocities = self.vy_integrator.step(self.y_velocities)
            #    self.z_velocities = self.vz_integrator.step(self.z_velocities)
            #    self.x_positions = self.x_integrator.step(self.x_positions)
            #    self.y_positions = self.y_integrator.step(self.y_positions)
            #    self.z_positions = self.z_integrator.step(self.z_positions)

            ##print(self.vx_integrator.step(self.x_velocities))
            ##print(self.vy_integrator.step(self.y_velocities))
            ##print(self.vz_integrator.step(self.z_velocities))
            ##print(self.x_integrator.step(self.x_positions))
            ##print(self.y_integrator.step(self.y_positions))
            ##print(self.z_integrator.step(self.z_positions))

            #self.acceleration_coeffs = self.set_acceleration_coeffs()
            #self.update_data()

            ##for i in range(self.num_bodies):
            ##    self.data['vx'][i].append(self.x_velocities[i])
            ##    self.data['vy'][i].append(self.y_velocities[i])
            ##    self.data['vz'][i].append(self.z_velocities[i])
            ##    self.data['x'][i].append(self.x_positions[i])
            ##    self.data['y'][i].append(self.y_positions[i])
            ##    self.data['z'][i].append(self.z_positions[i])
            ##    self.data['ac'][i].append(self.acceleration_coeffs[i])



    def get_energy(self):
        """Returns the sum of current kinetic and potential energies
        """
        return self.get_kinetic_energy() + self.get_potential_energy()

    def get_kinetic_energy(self):
        """Returns the current kinetic energy
        """
        energy = []
        total_energy = 0
        for i in range(self.num_bodies):
            velocity = abs(self.x_velocities[i]) \
                     + abs(self.y_velocities[i]) + abs(self.z_velocities[i])
            total_energy += (0.5)*self.masses[i]*velocity**2

        return total_energy

    def get_potential_energy(self):
        """Returns the current potential energy
        """
        G = self.consts['G']
        n = self.num_bodies
        new_mat = np.zeros(shape=(n, n))
        for i in range(n):
            for j in range(n):
                # Protecting against zero division
                if self.radii[i][j] == 0:
                    new_mat[i][j] = 0
                else:
                    new_mat[i][j] = G*self.masses[i]*self.masses[j]/self.radii[i][j]

        return -np.sum(np.tril(new_mat))

    def set_acceleration_coeffs(self):
        """Returns the coefficients G*m/r**3 for each body and sums all of the
        accelerations for each body
        """
        self.radii = self.set_radii()
        G = self.consts['G']
        acceleration_coeffs = [
            #sum([((4*np.pi**2)/(r**3) if r != 0 else 0)
            sum([(G*m/(r**3) if r != 0 else 0)
                 for m, r in zip(self.masses, radii)]) for radii in self.radii
        ]

        #if self.printer:
        #    if self.iterator <= 5:
        #        print([[self.consts['G']*m/np.power(r, 3) if r != 0 else 0
        #               for m, r in zip(self.masses, radii)]
        #            for radii in self.radii]
        #        )
        #        print("Acceleration Coefficients")
        #        print("----------------------------------")
        #        for i in acceleration_coeffs:
        #            print(i)
        #        self.iterator += 1

        return acceleration_coeffs

    def set_positions(self, coordinate):
        """Sets up the appropriate list for x, y, or z coordinates
        for the system.
        """
        coord_dict = {'x':0, 'y':1, 'z':2}
        positions = [body.position[coord_dict[coordinate]] for body in self.body_list]

        return positions

    def set_velocities(self, coordinate):
        """Sets up the appropriate list for x, y, or z velocities
        for the system.
        """
        coord_dict = {'x':0, 'y':1, 'z':2}
        velocities = [body.velocity[coord_dict[coordinate]] for body in self.body_list]

        return velocities

    def set_radii(self):
        """Computes the radius separating each body from every
        other body.
        """
        radii = [[None for i in range(self.num_bodies)] for i in range(self.num_bodies)]

        for i in range(self.num_bodies):
            for j in range(self.num_bodies):
                radii[i][j] = np.sqrt(float(
                    (self.x_positions[j] - self.x_positions[i])**2
                    + (self.y_positions[j] - self.y_positions[i])**2
                    + (self.z_positions[j] - self.z_positions[i])**2
                    ))

                #if self.printer:
                #    if self.iterator <= 5:
                #        print("Radii")

                #self.iterator += 1

        return radii

    # ----------------------------------------------------------------------- #
    # Update Methods
    # ----------------------------------------------------------------------- #
    def update_data(self):
        for i in range(self.num_bodies):
            self.data['vx'][i].append(self.x_velocities[i])
            self.data['vy'][i].append(self.y_velocities[i])
            self.data['vz'][i].append(self.z_velocities[i])

            self.data['x'][i].append(self.x_positions[i])
            self.data['y'][i].append(self.y_positions[i])
            self.data['z'][i].append(self.z_positions[i])


            self.data['ac'][i].append(self.acceleration_coeffs[i])

    def update_positions(self, new_positions_list, coord):
        if coord == 'x':
            self.x_positions = new_positions_list
        elif coord == 'y':
            self.y_positions = new_positions_list
        elif coord == 'z':
            self.y_positions = new_positions_list

    def update_velocities(self, new_velocities_list, coord):
        if coord == 'x':
            self.x_velocities = new_velocities_list
        elif coord == 'y':
            self.y_velocities = new_velocities_list
        elif coord == 'z':
            self.y_velocities = new_velocities_list

    def update_radii(self):
        self.radii = self.set_radii()

    def update_acceleration_coeffs(self):
        self.acceleration_coeffs = self.set_acceleration_coeffs()

    # ----------------------------------------------------------------------- #
    # Get positions
    # ----------------------------------------------------------------------- #
    def get_x_velocity(self, dt, x, i):
        return self.x_velocities[i]

    def get_y_velocity(self, dt, y, i):
        return self.y_velocities[i]

    def get_z_velocity(self, dt, z, i):
        return self.z_velocities[i]

    # ----------------------------------------------------------------------- #
    # Get accelerations
    # ----------------------------------------------------------------------- #
    def compute_x_acceleration(self, dt, v, i):
        new_pos = self.x_positions[i] + v*dt
        return -1.*self.acceleration_coeffs[i]*new_pos

    def compute_y_acceleration(self, dt, v, i):
        new_pos = self.y_positions[i] + v*dt
        return -1.*self.acceleration_coeffs[i]*new_pos

    def compute_z_acceleration(self, dt, v, i):
        new_pos = self.z_positions[i] + v*dt
        return -1.*self.acceleration_coeffs[i]*new_pos
